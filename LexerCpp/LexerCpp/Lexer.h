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
        dfa.add_transition(0, '(', token_to_int(Token::Kind::LeftParen));
        dfa.add_transition(0, ')', token_to_int(Token::Kind::RightParen));
        dfa.add_transition(0, '[', token_to_int(Token::Kind::LeftSquare));
        dfa.add_transition(0, ']', token_to_int(Token::Kind::RightSquare));
        dfa.add_transition(0, '{', token_to_int(Token::Kind::LeftCurly));
        dfa.add_transition(0, '}', token_to_int(Token::Kind::RightCurly));
        dfa.add_transition(0, '<', token_to_int(Token::Kind::LessThan));
        dfa.add_transition(0, '>', token_to_int(Token::Kind::GreaterThan));
        dfa.add_transition(0, '=', token_to_int(Token::Kind::Equal));
        dfa.add_transition(0, '+', token_to_int(Token::Kind::Plus));
        dfa.add_transition(0, '-', token_to_int(Token::Kind::Minus));
        dfa.add_transition(0, '*', token_to_int(Token::Kind::Asterisk));
        dfa.add_transition(0, '-', token_to_int(Token::Kind::Slash));
        dfa.add_transition(0, '/', token_to_int(Token::Kind::Hash));
        dfa.add_transition(0, '#', token_to_int(Token::Kind::Dot));
        dfa.add_transition(0, ',', token_to_int(Token::Kind::Comma));
        dfa.add_transition(0, ':', token_to_int(Token::Kind::Colon));
        dfa.add_transition(0, ';', token_to_int(Token::Kind::Semicolon));
        dfa.add_transition(0, '\'', token_to_int(Token::Kind::SingleQuote));
        dfa.add_transition(0, '"', token_to_int(Token::Kind::DoubleQuote));
       
        dfa.add_transition(0, '=', token_to_int(Token::Kind::Equal));
        dfa.add_transition(0, ' ', token_to_int(Token::Kind::Whitespace));
        dfa.add_transition(token_to_int(Token::Kind::Whitespace), ' ', token_to_int(Token::Kind::Whitespace));
        
        for (char c = 'a'; c <= 'z'; c++)
        {
            dfa.add_transition(0, c, token_to_int(Token::Kind::Identifier));
            dfa.add_transition(token_to_int(Token::Kind::Identifier), c, token_to_int(Token::Kind::Identifier));
        }

        for (char c = '0'; c <= '9'; c++)
        {
            dfa.add_transition(token_to_int(Token::Kind::Identifier), c, token_to_int(Token::Kind::Identifier));
        }

        for (char c = '0'; c <= '9'; c++)
        {
            dfa.add_transition(0, c, token_to_int(Token::Kind::Number));
            dfa.add_transition(token_to_int(Token::Kind::Number), c, token_to_int(Token::Kind::Number));
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
    bool is_end() {
        return *m_beg == '\0';
    }

    const char* m_beg = nullptr;
    DFA<char> dfa;
};

