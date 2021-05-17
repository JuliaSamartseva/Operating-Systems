#pragma once
#include "Token.h"
#include "DFA.h"

class Lexer {
public:
    Lexer(const char* beg) : m_beg{ beg }, dfa{0, false} {
        // Adding DFA state for each of the tokens
        for (int i = 101; i < 127; i++) {
            dfa.add_state(i, true);
        }
        // TODO: add all token definitions
        dfa.add_transition(0, '(', toUnderlyingType(Token::Kind::LeftParen));
        dfa.add_transition(0, '=', toUnderlyingType(Token::Kind::Equal));
        dfa.add_transition(0, ' ', toUnderlyingType(Token::Kind::Whitespace));
        dfa.add_transition(toUnderlyingType(Token::Kind::Whitespace), ' ', toUnderlyingType(Token::Kind::Whitespace));
        
        for (char c = 'a'; c <= 'z'; c++)
        {
            dfa.add_transition(0, c, toUnderlyingType(Token::Kind::Identifier));
            dfa.add_transition(toUnderlyingType(Token::Kind::Identifier), c, toUnderlyingType(Token::Kind::Identifier));
        }

        for (char c = '0'; c <= '9'; c++)
        {
            dfa.add_transition(toUnderlyingType(Token::Kind::Identifier), c, toUnderlyingType(Token::Kind::Identifier));
        }

        for (char c = '0'; c <= '9'; c++)
        {
            dfa.add_transition(0, c, toUnderlyingType(Token::Kind::Number));
            dfa.add_transition(toUnderlyingType(Token::Kind::Number), c, toUnderlyingType(Token::Kind::Number));
        }
    }

    Token next();

private:
    char peek() const { return *m_beg; }
    char peek_next() { return *(++m_beg); }
    char get() { return *m_beg++; }
    bool current_not_null() { 
        if (*m_beg == '\0') return false;
        return m_beg != nullptr; 
    }

    const char* m_beg = nullptr;
    DFA<char> dfa;
};

