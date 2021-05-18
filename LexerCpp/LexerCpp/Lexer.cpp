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
