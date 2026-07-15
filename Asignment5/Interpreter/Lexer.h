#pragma once
#include <string>
#include <vector>
#include "Token.h"

class Lexer {
private:
	std::string input;
	std::size_t position;
	char currentChar() const;
	char nextChar() const;
	bool isAtEnd() const;
	void advance();
	void skipWhitespace();
	Token readNumber();
	Token readIdentifier();

public:
	explicit Lexer(const std::string& inputText);
	std::vector<Token> tokenize();
};