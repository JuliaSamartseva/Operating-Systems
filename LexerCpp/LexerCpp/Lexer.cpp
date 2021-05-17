#include "Lexer.h"

Token Lexer::next() {
   
    dfa.reset();
    const char* start_lexeme = m_beg;
    while (current_not_null()) {
        dfa.input(peek());
        if (dfa.is_accepting() && !dfa.is_accepting(peek_next())) {
            // Found token, returning the result.
            return Token((Token::Kind)dfa.state(), start_lexeme, m_beg);
        }
        get();
    }

    return Token((Token::Kind)dfa.state(), start_lexeme, m_beg);
}
