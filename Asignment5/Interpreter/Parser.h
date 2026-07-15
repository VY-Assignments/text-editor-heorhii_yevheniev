#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Token.h"
#include "Expression.h"

enum class StatementType {
	expression,
	variable_declaration,
	function_declaration
};

struct ParsedStatement {
	StatementType type;
	std::string name;
	std::vector<std::string> parameters;
	std::unique_ptr<Expression> expression;

	ParsedStatement(
		StatementType statementType,
		const std::string& statementName,
		std::unique_ptr<Expression> statementExpression
	);

	ParsedStatement(
		StatementType statementType,
		const std::string& statementName,
		const std::vector<std::string>& functionParameters,
		std::unique_ptr<Expression> statementExpression
	);
};

class Parser {
private:
	std::vector<Token> tokens;
	std::size_t position;
	const Token& currentToken() const;
	const Token& previousToken() const;
	bool isAtEnd() const;
	bool check(TokenType type) const;
	bool match(TokenType type);
	void advance();
	const Token& consume(TokenType type, const std::string& message);
	std::unique_ptr<Expression> parseExpression();
	std::unique_ptr<Expression> parseAddition();
	std::unique_ptr<Expression> parseMultiplication();
	std::unique_ptr<Expression> parseUnary();
	std::unique_ptr<Expression> parsePrimary();
	std::unique_ptr<Expression> parseFunctionCall(const std::string& functionName);
public:
	explicit Parser(const std::vector<Token>& tokenList);
	ParsedStatement parse();
};