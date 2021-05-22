// LexerCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "Lexer.h"

int main()
{

    auto code = "int #defdfjdhjkf int error = 2";
    std::cout << "List of tokens: \n";
    Lexer lex(code);
    for (auto token = lex.next();
        not token.is(Token::Kind::End);
        token = lex.next()) {
        if (token.is(Token::Kind::Invalid)) {
            std::cout << "Found invalid token";
            break;
        } else {
            std::cout << "(" << Token::toString(token.kind()) << ", \"" << token.lexeme()
                << "\")\n";
        }
    }
}
