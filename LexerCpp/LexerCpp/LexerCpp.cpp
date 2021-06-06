// LexerCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include "Lexer.h"

int main()
{
    ifstream file("cpp_code.txt");
    std::cout << "List of tokens: \n";
    std::string code((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    Lexer lex(code.c_str());
    for (auto token = lex.next();
        not token.is(Token::Kind::End);
        token = lex.next()) {
        if (token.is(Token::Kind::Invalid)) {
            std::cout << "Found invalid token";
            break;
        }
        else {
            std::cout << "(" << Token::toString(token.kind()) << ", \"" << token.lexeme()
                << "\")\n";
        }
    }
}