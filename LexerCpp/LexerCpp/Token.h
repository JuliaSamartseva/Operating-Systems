#pragma once
#include <string_view>

class Token {
public:
	enum class Kind {
		Invalid = 100,
		// numeric constants
		Number,
		//TODO: add floating point constant
		//TODO: literals and symbolic constants
		//TODO: preprocessor directives
		//TODO: comments
		//TODO: keywords
		// identifiers
		Identifier,
		//TODO: operators
		DirectAssignment,
		AdditionAssignment,
		SubtractionAssignment,
		MultiplicationAssignment,
		DivisionAssignment,
		// punctuation marks
		LeftParen,
		RightParen,
		LeftSquare,
		RightSquare,
		LeftCurly,
		RightCurly,
		LessThan,
		GreaterThan,
		Plus,
		Minus,
		Multiply,
		Divide,
		Hash,
		Dot,
		Comma,
		Colon,
		Semicolon,
		SingleQuote,
		DoubleQuote,
		Comment,
		Pipe,
		End,
		Whitespace,
	};

	static std::string toString(Kind kind) {
		switch (kind)
		{
		case Kind::Invalid: return "Invalid token";
		case Kind::Number: return "Number";
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
		case Kind::Comment: return "Comment";
		case Kind::Pipe: return "Pipe";
		case Kind::End: return "End";
		case Kind::Whitespace: return "Whitespace";
		default: return "[unknown token]";
		}
	}

	Token(Kind kind) : m_kind{ kind } {}

	Token(Kind kind, const char* beg, std::size_t len)
		: m_kind{ kind }, m_lexeme(beg, len) {}

	Token(Kind kind, const char* beg, const char* end)
		: m_kind{ kind }, m_lexeme(beg, std::distance(beg, end)) {}

	Kind kind() const { return m_kind; }

	void kind(Kind kind) { m_kind = kind; }

	bool is(Kind kind) const { return m_kind == kind; }

	bool is_not(Kind kind) const { return m_kind != kind; }

	bool is_one_of(Kind k1, Kind k2) const { return is(k1) || is(k2); }

	template <typename... Ts>
	bool is_one_of(Kind k1, Kind k2, Ts... ks) const {
		return is(k1) || is_one_of(k2, ks...);
	}

	std::string_view lexeme() const { return m_lexeme; }

	void lexeme(std::string_view lexeme) {
		m_lexeme = std::move(lexeme);
	}

private:
	Kind m_kind{};
	std::string_view m_lexeme{};
};

template<typename E>
constexpr auto token_to_int(E e)
{
	return static_cast<typename std::underlying_type<E>::type>(e);
}