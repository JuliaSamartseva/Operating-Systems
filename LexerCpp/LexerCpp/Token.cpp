#include "Token.h"

std::string Token::toString(Kind kind) {
	switch (kind)
	{
	case Kind::Invalid: return "Invalid token";
	case Kind::IntegerLiteral: return "Integer literal";
	case Kind::FloatingPointLiteral: return "Floating point literal";
	case Kind::Identifier: return "Identifier";
	case Kind::LeftParen: return "Left Parenthesis";
	case Kind::RightParen: return "Right Parenthesis";
	case Kind::LeftSquare: return "Left Square Parenthesis";
	case Kind::RightSquare: return "Right Square Parenthesis";
	case Kind::LeftCurly: return "Left Curly Parenthesis";
	case Kind::RightCurly: return "Right Curly Parenthesis";
	case Kind::LessThan: return "Less than";
	case Kind::GreaterThan: return "Greater than";
	case Kind::DirectAssignment: return "Direct assignment operator";
	case Kind::AdditionAssignment: return "Addition assignment operator";
	case Kind::SubtractionAssignment: return "Substration assignment operator";
	case Kind::MultiplicationAssignment: return "Multiplication assignment operator";
	case Kind::DivisionAssignment: return "Division assignment operator";
	case Kind::Equals: return "Equals";
	case Kind::Plus: return "Plus";
	case Kind::Minus: return "Minus";
	case Kind::Multiply: return "Multiply";
	case Kind::Divide: return "Divide";
	case Kind::Hash: return "Hash";
	case Kind::Dot: return "Dot";
	case Kind::Comma: return "Comma";
	case Kind::Colon: return "Colon";
	case Kind::Semicolon: return "Semicolon";
	case Kind::SingleQuote: return "Single Quote";
	case Kind::DoubleQuote: return "Double Quote";
	case Kind::CppStyleComment: return "C++ Style Comment";
	case Kind::CStyleComment: return "C Style Comment";
	case Kind::Pipe: return "Pipe";
	case Kind::End: return "End";
	case Kind::Whitespace: return "Whitespace";
	case Kind::NewLine: return "New line";
	case Kind::Keyword: return "Keyword";
	case Kind::PreprocessorDirectives: return "Preprocessor directive";
	case Kind::StringLiteral: return "String Literal";
	default: return "[unknown token]";
	}
}
