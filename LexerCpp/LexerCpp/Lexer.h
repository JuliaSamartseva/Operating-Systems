#pragma once
#include "Token.h"
#include "DFA.h"
#include "Trie.h"
#include <optional>
#include <string_view>
#include <unordered_set>

class Lexer {
public:
	Lexer(const char* beg) : m_beg{ beg }, dfa{ 0, false } {
		// Adding DFA state for each of the tokens
		for (int i = 1; i < 35; i++) {
			dfa.add_state(i, true);
		}
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

		// Handle identifiers as a set of chars/numbers
		for (char c = 'a'; c <= 'z'; c++) {
			dfa.add_transition(0, c, token_to_int(Token::Kind::Identifier));
			dfa.add_transition(token_to_int(Token::Kind::Identifier), c, token_to_int(Token::Kind::Identifier));
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

		std::vector<std::string> words =
		{"alignas", "alignof", "and", "and_eq", "asm", "atomic_cancel",
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
		for (std::string word : words) {
			reserved_words.insert(word);
		}
	}

	Token next();

private:
	char peek() const { return *m_beg; }
	char peek_next() { return *(m_beg + 1); }
	char get() { return *m_beg++; }
	bool current_not_null() {
		if (*m_beg == '\0') return false;
		return m_beg != nullptr;
	}
	bool is_end() {
		return *m_beg == '\0';
	}
	Token get_token_from_dfa(const char* start_lexeme, const char* end_lexeme);
	std::optional<Token::Kind> check_if_reserved_word(std::string input);

	Trie reserved_words;
	const char* m_beg = nullptr;
	DFA<char> dfa;
};

