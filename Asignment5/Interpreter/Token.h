#pragma once
#include <string>

enum class TokenType {
	number,
	identifier,
	var,
	def,
	plus,
	minus,
	multiply,
	divide,
	left_paren,
	right_paren,
	left_brace,
	right_brace,
	comma,
	equal,
	end
};

struct Token {
	TokenType type;
	std::string text;
	double numberValue;

	Token(TokenType tokenType, const std::string& tokenText)
		:type(tokenType),text(tokenText),numberValue(0.0){ }

	Token(TokenType tokenType,const std::string& tokenText, double value)
		:type(tokenType),text(tokenText),numberValue(value){ }

};

inline std::string tokenTypeToString(TokenType type) {
	switch (type) {
	case TokenType::number:
		return "number";
	case TokenType::identifier:
		return "identifier";
	case TokenType::var:
		return "var";
	case TokenType::def:
		return "def";
	case TokenType::plus:
		return "plus";
	case TokenType::minus:
		return "minus";
	case TokenType::multiply:
		return "multiply";
	case TokenType::divide:
		return "divide";
	case TokenType::left_paren:
		return "left_paren";
	case TokenType::right_paren:
		return "right_paren";
	case TokenType::left_brace:
		return "left_brace";
	case TokenType::right_brace:
		return "right_brace";
	case TokenType::comma:
		return "comma";
	case TokenType::equal:
		return "equal";
	case TokenType::end:
		return "end";
	}
	return "unknown";
}