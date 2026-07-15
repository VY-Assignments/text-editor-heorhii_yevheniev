#include "Lexer.h"
#include <cctype>
#include <stdexcept>

Lexer::Lexer(const std::string& inputText)
	:input(inputText),position(0){ }

char Lexer::currentChar() const {
	if (isAtEnd()) {
		return '\0';
	}
	return input[position];
}

char Lexer::nextChar() const {
	if (position + 1 >= input.length()) {
		return '\0';
	}
	return input[position + 1];
}

bool Lexer::isAtEnd() const {
	return position >= input.length();
}

void Lexer::advance() {
	if (!isAtEnd()) {
		position++;
	}
}

void Lexer::skipWhitespace() {
	while (!isAtEnd() &&
		std::isspace(static_cast<unsigned char>(currentChar()))) {
		advance();
	}
}

Token Lexer::readNumber() {
	std::size_t startPosition = position;
	bool hasDecimalPoint = false;
	while (!isAtEnd()) {
		char character = currentChar();
		if (std::isdigit(static_cast<unsigned char>(character))) {
			advance();
		}
		else if (character == '.' && !hasDecimalPoint) {
			hasDecimalPoint = true;
			advance();
		}
		else {
			break;
		}
	}
	std::string numberText = input.substr(startPosition, position - startPosition);
	try {
		double value = std::stod(numberText);
		return Token(TokenType::number, numberText, value);
	}
	catch (...) {
		throw std::runtime_error(
			"Invalid number: " + numberText
		);
	}
}

Token Lexer::readIdentifier() {
	std::size_t startPosition = position;
	while (!isAtEnd()) {
		char character = currentChar();
		if (std::isalnum(static_cast<unsigned char>(character)) ||
			character == '_') {
			advance();
		}
		else {
			break;
		}
	}
	std::string identifier = input.substr(startPosition, position - startPosition);
	if (identifier == "var") {
		return Token(TokenType::var, identifier);
	}

	if (identifier == "def") {
		return Token(TokenType::def, identifier);
	}
	return Token(TokenType::identifier, identifier);
}

std::vector<Token> Lexer::tokenize() {
	std::vector<Token> tokens;
	while (!isAtEnd()) {
		skipWhitespace();
		if (isAtEnd()) {
			break;
		}
		char character = currentChar();
		if (std::isdigit(static_cast<unsigned char>(character)) ||
			(character == '.' &&
				std::isdigit(static_cast<unsigned char>(nextChar())))) {
			tokens.push_back(readNumber());
			continue;
		}
		if (std::isalpha(static_cast<unsigned char>(character)) ||
			character == '_') {
			tokens.push_back(readIdentifier());
			continue;
		}
		switch (character) {
		case '+':
			tokens.emplace_back(TokenType::plus, "+");
			advance();
			break;
		case '-':
			tokens.emplace_back(TokenType::minus, "-");
			advance();
			break;
		case '*':
			tokens.emplace_back(TokenType::multiply, "*");
			advance();
			break;
		case '/':
			tokens.emplace_back(TokenType::divide, "/");
			advance();
			break;
		case '(':
			tokens.emplace_back(TokenType::left_paren, "(");
			advance();
			break;
		case ')':
			tokens.emplace_back(TokenType::right_paren, ")");
			advance();
			break;
		case '{':
			tokens.emplace_back(TokenType::left_brace, "{");
			advance();
			break;
		case '}':
			tokens.emplace_back(TokenType::right_brace, "}");
			advance();
			break;
		case ',':
			tokens.emplace_back(TokenType::comma, ",");
			advance();
			break;
		case '=':
			tokens.emplace_back(TokenType::equal, "=");
			advance();
			break;
		default:
			throw std::runtime_error(
				"wrong character: " + std::string(1, character)
			);
		}
	}
	tokens.emplace_back(TokenType::end, "");
	return tokens;
}