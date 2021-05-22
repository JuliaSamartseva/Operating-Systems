#pragma once
#include "Token.h"
#include "DFA.h"
#include "Trie.h"
#include <optional>
#include <string_view>
#include <unordered_set>

class Lexer {
public:
	Lexer(const char* beg);

	Token next();

private:
	char peek() const;
	char peek_next();
	char get();
	bool current_not_null();
	bool is_end();
	Token get_token_from_dfa(const char* start_lexeme, const char* end_lexeme);
	std::optional<Token::Kind> check_if_reserved_word(std::string input);
	std::optional<Token::Kind> check_if_preprocessor_directive(std::string input);

	Trie reserved_words;
	Trie preprocessor_directives;
	const char* m_beg = nullptr;
	DFA<char> dfa;
};

