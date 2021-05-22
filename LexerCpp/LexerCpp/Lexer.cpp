#include "Lexer.h"

inline std::string as_string(std::string_view v) {
    return { v.data(), v.size() };
}

Lexer::Lexer(const char* beg) {
	this->m_beg = beg;
	this->dfa = DFA<char>(0, false);

	// Adding DFA state for each of the tokens
	for (int i = 1; i < 38; i++) {
		dfa.add_state(i, true);
	}
	dfa.add_state(token_to_int(Token::Kind::EndComment), false);
	dfa.add_state(token_to_int(Token::Kind::CStyleCommentStart), true);
	dfa.add_state(token_to_int(Token::Kind::CStyleCommentEnd), true);
	dfa.add_state(token_to_int(Token::Kind::StringLiteralStart), true);

	// all one-symbol tokens
	dfa.add_transition(0, '(', token_to_int(Token::Kind::LeftParen));
	dfa.add_transition(0, ')', token_to_int(Token::Kind::RightParen));
	dfa.add_transition(0, '[', token_to_int(Token::Kind::LeftSquare));
	dfa.add_transition(0, ']', token_to_int(Token::Kind::RightSquare));
	dfa.add_transition(0, '{', token_to_int(Token::Kind::LeftCurly));
	dfa.add_transition(0, '}', token_to_int(Token::Kind::RightCurly));
	dfa.add_transition(0, '<', token_to_int(Token::Kind::LessThan));
	dfa.add_transition(0, '>', token_to_int(Token::Kind::GreaterThan));
	dfa.add_transition(0, '=', token_to_int(Token::Kind::DirectAssignment));
	dfa.add_transition(0, '+', token_to_int(Token::Kind::Plus));
	dfa.add_transition(0, '-', token_to_int(Token::Kind::Minus));
	dfa.add_transition(0, '*', token_to_int(Token::Kind::Multiply));
	dfa.add_transition(0, '/', token_to_int(Token::Kind::Divide));
	dfa.add_transition(0, '#', token_to_int(Token::Kind::Hash));
	dfa.add_transition(0, '.', token_to_int(Token::Kind::Dot));
	dfa.add_transition(0, ',', token_to_int(Token::Kind::Comma));
	dfa.add_transition(0, ':', token_to_int(Token::Kind::Colon));
	dfa.add_transition(0, ';', token_to_int(Token::Kind::Semicolon));
	dfa.add_transition(0, '\'', token_to_int(Token::Kind::SingleQuote));
	dfa.add_transition(0, '"', token_to_int(Token::Kind::DoubleQuote));
	dfa.add_transition(0, ' ', token_to_int(Token::Kind::Whitespace));
	dfa.add_transition(0, '\n', token_to_int(Token::Kind::NewLine));

	// Handle several whitespaces
	dfa.add_transition(token_to_int(Token::Kind::Whitespace), ' ', token_to_int(Token::Kind::Whitespace));

	// Assignment operators
	dfa.add_transition(token_to_int(Token::Kind::Plus), '=', token_to_int(Token::Kind::AdditionAssignment));
	dfa.add_transition(token_to_int(Token::Kind::Minus), '=', token_to_int(Token::Kind::SubtractionAssignment));
	dfa.add_transition(token_to_int(Token::Kind::Multiply), '=', token_to_int(Token::Kind::MultiplicationAssignment));
	dfa.add_transition(token_to_int(Token::Kind::Divide), '=', token_to_int(Token::Kind::DivisionAssignment));
	dfa.add_transition(token_to_int(Token::Kind::DirectAssignment), '=', token_to_int(Token::Kind::Equals));

	// Handle identifiers as a set of chars/numbers, preprocessor directives, comments
	for (char c = 'a'; c <= 'z'; c++) {
		dfa.add_transition(0, c, token_to_int(Token::Kind::Identifier));
		dfa.add_transition(token_to_int(Token::Kind::Identifier), c, token_to_int(Token::Kind::Identifier));

		dfa.add_transition(token_to_int(Token::Kind::Hash), c, token_to_int(Token::Kind::PreprocessorDirectives));
		dfa.add_transition(token_to_int(Token::Kind::PreprocessorDirectives), c, token_to_int(Token::Kind::PreprocessorDirectives));
	}
	for (char c = 'A'; c <= 'Z'; c++) {
		dfa.add_transition(0, c, token_to_int(Token::Kind::Identifier));
		dfa.add_transition(token_to_int(Token::Kind::Identifier), c, token_to_int(Token::Kind::Identifier));

		dfa.add_transition(token_to_int(Token::Kind::Hash), c, token_to_int(Token::Kind::PreprocessorDirectives));
		dfa.add_transition(token_to_int(Token::Kind::PreprocessorDirectives), c, token_to_int(Token::Kind::PreprocessorDirectives));
	}
	for (char c = '0'; c <= '9'; c++) {
		dfa.add_transition(token_to_int(Token::Kind::Identifier), c, token_to_int(Token::Kind::Identifier));
	}

	// Handle int numbers as a set of digits
	// Floating point literals
	for (char c = '0'; c <= '9'; c++) {
		dfa.add_transition(0, c, token_to_int(Token::Kind::IntegerLiteral));
		dfa.add_transition(token_to_int(Token::Kind::IntegerLiteral), '.', token_to_int(Token::Kind::FloatingPointLiteral));
		dfa.add_transition(token_to_int(Token::Kind::FloatingPointLiteral), c, token_to_int(Token::Kind::FloatingPointLiteral));
		dfa.add_transition(token_to_int(Token::Kind::IntegerLiteral), c, token_to_int(Token::Kind::IntegerLiteral));
	}

	// Handle C++ comments
	dfa.add_transition(token_to_int(Token::Kind::Divide), '/', token_to_int(Token::Kind::CppStyleComment));
	dfa.add_transition(token_to_int(Token::Kind::CppStyleComment), '\n', token_to_int(Token::Kind::EndComment));
	dfa.add_transition(token_to_int(Token::Kind::CppStyleComment), '\0', token_to_int(Token::Kind::EndComment));
	dfa.add_any_transition(token_to_int(Token::Kind::CppStyleComment), token_to_int(Token::Kind::CppStyleComment));

	// Handle C comments
	dfa.add_transition(token_to_int(Token::Kind::Divide), '*', token_to_int(Token::Kind::CStyleCommentStart));
	dfa.add_any_transition(token_to_int(Token::Kind::CStyleCommentStart), token_to_int(Token::Kind::CStyleCommentStart));
	dfa.add_transition(token_to_int(Token::Kind::CStyleCommentStart), '*', token_to_int(Token::Kind::CStyleCommentEnd));
	dfa.add_any_transition(token_to_int(Token::Kind::CStyleCommentEnd), token_to_int(Token::Kind::CStyleCommentStart));
	dfa.add_transition(token_to_int(Token::Kind::CStyleCommentEnd), '*', token_to_int(Token::Kind::CStyleCommentEnd));
	dfa.add_transition(token_to_int(Token::Kind::CStyleCommentEnd), '/', token_to_int(Token::Kind::CStyleComment));

	// Handle String literals
	dfa.add_any_transition(token_to_int(Token::Kind::DoubleQuote), token_to_int(Token::Kind::StringLiteralStart));
	dfa.add_any_transition(token_to_int(Token::Kind::StringLiteralStart), token_to_int(Token::Kind::StringLiteralStart));
	dfa.add_transition(token_to_int(Token::Kind::StringLiteralStart), '\"', token_to_int(Token::Kind::StringLiteral));

	// Handle keywords, preprocessor directives
	std::vector<std::string> keywords_array =
	{ "alignas", "alignof", "and", "and_eq", "asm", "atomic_cancel",
		"atomic_commit", "atomic_noexcept", "auto", "bitand", "bitor", "bool",
		"break", "case", "catch", "char", "char8_t", "char16_t", "char32_t",
		"class", "compl", "concept", "const", "consteval", "constexpr",
		"constinit", "const_cast", "continue", "co_await", "co_return",
		"co_yield", "decltype", "default", "delete", "do", "double",
		"dynamic_cast", "else", "enum", "explicit", "export", "extern",
		"false", "float", "for", "friend", "goto", "if", "inline", "int",
		"long", "mutable", "namespace", "new", "noexcept", "not", "not_eq",
		"nullptr", "operator", "or", "or_eq", "private", "protected",
		"public", "reflexpr", "register", "reinterpret_cast", "requires",
		"return", "short", "signed", "sizeof", "static", "static_assert",
		"static_cast", "struct", "switch", "synchronized", "template", "this",
		"thread_local", "throw", "true", "try", "typedef", "typeid",
		"typename", "union", "unsigned", "using", "virtual", "void",
		"volatile", "wchar_t", "while", "xor", "xor_eq" };
	for (std::string word : keywords_array) {
		reserved_words.insert(word);
	}

	std::vector<std::string> preprocessor_directives_array =
	{ "#if", "#elif", "#else", "#endif", "#ifdef", "#ifndef", "#define",
		"#undef", "#include", "#line", "#error", "#pragma" };
	for (std::string word : preprocessor_directives_array) {
		preprocessor_directives.insert(word);
	}
}

Token Lexer::next() {
    dfa.reset();
    const char* start_lexeme = m_beg;
    while (current_not_null()) {
        dfa.input(peek());
        if (dfa.is_accepting() && !dfa.is_accepting(peek_next())) {
            // Found token, returning the result.
            get();
            return get_token_from_dfa(start_lexeme, m_beg);
        }
        get();
    }

    if (is_end()) return Token(Token::Kind::End, m_beg, 1);
    return get_token_from_dfa(start_lexeme, m_beg);
}

char Lexer::peek() const { return *m_beg; }

char Lexer::peek_next() { return *(m_beg + 1); }

char Lexer::get() { return *m_beg++; }

bool Lexer::current_not_null() {
    if (*m_beg == '\0') return false;
    return m_beg != nullptr;
}

bool Lexer::is_end() {
    return *m_beg == '\0';
}

Token Lexer::get_token_from_dfa(const char* start_lexeme, const char* end_lexeme) {
    if ((Token::Kind)dfa.state() == Token::Kind::PreprocessorDirectives) {
        std::string_view word = string_view(start_lexeme, std::distance(start_lexeme, end_lexeme));
        if (auto kind = check_if_preprocessor_directive(as_string(word)); kind) {
            return Token(*kind, start_lexeme, end_lexeme);
        } else return Token(Token::Kind::Invalid, start_lexeme, end_lexeme);
    }

    if ((Token::Kind)dfa.state() == Token::Kind::Identifier) {
        std::string_view word = string_view(start_lexeme, std::distance(start_lexeme, end_lexeme));
        if (auto kind = check_if_reserved_word(as_string(word)); kind) {
            return Token(*kind, start_lexeme, end_lexeme);
        }
    }
    return Token((Token::Kind)dfa.state(), start_lexeme, end_lexeme);
}

// returns Token::Kind if the word is reserved, or std::nullopt if the word is not reserved
std::optional<Token::Kind> Lexer::check_if_reserved_word(std::string input) {
    if (reserved_words.search(input)) return Token::Kind::Keyword;
    return std::nullopt;
}

std::optional<Token::Kind> Lexer::check_if_preprocessor_directive(std::string input) {
    if (preprocessor_directives.search(input)) return Token::Kind::PreprocessorDirectives;
    return std::nullopt;
}