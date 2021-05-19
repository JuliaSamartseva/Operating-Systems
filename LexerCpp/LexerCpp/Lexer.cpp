#include "Lexer.h"

Token Lexer::next() {
    dfa.reset();
    const char* start_lexeme = m_beg;
    while (current_not_null()) {
        dfa.input(peek());
        if (dfa.is_accepting() && !dfa.is_accepting(peek_next())) {
            // Found token, returning the result.
            get();
            return Token((Token::Kind)dfa.state(), start_lexeme, m_beg);
        }
        get();
    }

    if (is_end()) return Token(Token::Kind::End, m_beg, 1);
    return Token((Token::Kind)dfa.state(), start_lexeme, m_beg);
}

Token Lexer::get_token_from_dfa(char* start_lexeme, char* end_lexeme) {
    if ((Token::Kind)dfa.state() == Token::Kind::Identifier) {
        std::string_view word = string_view(start_lexeme, std::distance(start_lexeme, end_lexeme));
        if (auto kind = check_if_reserved_word(word); kind) {
            return Token(*kind, start_lexeme, end_lexeme);
        }
    }
    return Token((Token::Kind)dfa.state(), start_lexeme, end_lexeme);
}

// returns Token::Kind if the word is reserved, or std::nullopt if the word is not reserved
std::optional<Token::Kind> Lexer::check_if_reserved_word(std::string_view input) {
    return std::nullopt;
}
