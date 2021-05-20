#include "Lexer.h"

inline std::string as_string(std::string_view v) {
    return { v.data(), v.size() };
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