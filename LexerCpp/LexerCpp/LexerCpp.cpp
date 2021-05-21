// LexerCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "Lexer.h"

int main()
{

    auto code = "/* \n hello = 3 23 */ = 3";
    std::cout << "List of tokens: \n";
    Lexer lex(code);
    for (auto token = lex.next();
        not token.is_one_of(Token::Kind::End, Token::Kind::Invalid);
        token = lex.next()) {
        std::cout << "(" << Token::toString(token.kind()) << ", \"" << token.lexeme()
            << "\")\n";
    }
}
