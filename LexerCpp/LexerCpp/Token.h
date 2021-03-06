#pragma once
#include <string_view>

class Token {
public:
	enum class Kind {
		Invalid = 0,
		// numeric constants
		IntegerLiteral,
		FloatingPointLiteral,
		//literals and symbolic constants
		StringLiteral,
		// preprocessor directives
		PreprocessorDirectives,
		// comments
		CppStyleComment,
		CStyleComment,
		// keywords
		Keyword,
		// identifiers
		Identifier,
		// operators
		Equals,
		DirectAssignment,
		AdditionAssignment,
		SubtractionAssignment,
		MultiplicationAssignment,
		DivisionAssignment,
		Plus,
		Minus,
		Multiply,
		Divide,
		// punctuation marks
		LeftParen,
		RightParen,
		LeftSquare,
		RightSquare,
		LeftCurly,
		RightCurly,
		LessThan,
		GreaterThan,
		Hash,
		Dot,
		Comma,
		Colon,
		Semicolon,
		SingleQuote,
		DoubleQuote,
		Pipe,
		End,
		Whitespace,
		NewLine,
		CStyleCommentStart,
		CStyleCommentEnd,
		StringLiteralStart,
		EscapeNewLine,
		EndComment,
	};

	static const int states_num = 41;

	static std::string toString(Kind kind);

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