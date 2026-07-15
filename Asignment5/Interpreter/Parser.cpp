#include "Parser.h"
#include <stdexcept>
#include <utility>

ParsedStatement::ParsedStatement(
	StatementType statementType,
	const std::string& statementName,
	std::unique_ptr<Expression> statementExpression
)
	:type(statementType), name(statementName), expression(std::move(statementExpression)) {
}

ParsedStatement::ParsedStatement(
	StatementType statementType,
	const std::string& statementName,
	const std::vector<std::string>& functionParameters,
	std::unique_ptr<Expression> statementExpression
)
	:type(statementType),
	name(statementName),
	parameters(functionParameters),
	expression(std::move(statementExpression)) {
}

Parser::Parser(const std::vector<Token>& tokenList)
	:tokens(tokenList), position(0) {
}

const Token& Parser::currentToken() const {
	return tokens[position];
}

const Token& Parser::previousToken() const {
	return tokens[position - 1];
}

bool Parser::isAtEnd() const {
	return currentToken().type == TokenType::end;
}

bool Parser::check(TokenType type) const {
	return currentToken().type == type;
}

bool Parser::match(TokenType type) {
	if (!check(type)) {
		return false;
	}
	advance();
	return true;
}

void Parser::advance() {
	if (!isAtEnd()) {
		position++;
	}
}

const Token& Parser::consume(TokenType type, const std::string& message) {
	if (check(type)) {
		const Token& token = currentToken();
		advance();
		return token;
	}
	throw std::runtime_error(message);
}

ParsedStatement Parser::parse() {
	if (match(TokenType::var)) {
		const Token& name = consume(
			TokenType::identifier,
			"Expected variable name after 'var'"
		);
		consume(
			TokenType::equal,
			"Expected '=' after variable name"
		);
		std::unique_ptr<Expression> expression = parseExpression();
		if (!check(TokenType::end)) {
			throw std::runtime_error(
				"Unexpected token: " + currentToken().text
			);
		}
		return ParsedStatement(
			StatementType::variable_declaration,
			name.text,
			std::move(expression)
		);
	}
	if (match(TokenType::def)) {
		const Token& name = consume(
			TokenType::identifier,
			"Expected function name after 'def'"
		);
		consume(
			TokenType::left_paren,
			"Expected '(' after function name"
		);
		std::vector<std::string> parameters;
		if (!check(TokenType::right_paren)) {
			const Token& parameter = consume(
				TokenType::identifier,
				"Expected parameter name"
			);
			parameters.push_back(parameter.text);
			while (match(TokenType::comma)) {
				const Token& nextParameter = consume(
					TokenType::identifier,
					"Expected parameter name after ','"
				);
				parameters.push_back(nextParameter.text);
			}
		}
		consume(
			TokenType::right_paren,
			"Expected ')' after function parameters"
		);
		consume(
			TokenType::left_brace,
			"Expected '{' before function body"
		);
		std::unique_ptr<Expression> body = parseExpression();
		consume(
			TokenType::right_brace,
			"Expected '}' after function body"
		);
		if (!check(TokenType::end)) {
			throw std::runtime_error(
				"Unexpected token: " + currentToken().text
			);
		}
		return ParsedStatement(
			StatementType::function_declaration,
			name.text,
			parameters,
			std::move(body)
		);
	}
	std::unique_ptr<Expression> expression = parseExpression();
	if (!check(TokenType::end)) {
		throw std::runtime_error(
			"Unexpected token: " + currentToken().text
		);
	}
	return ParsedStatement(
		StatementType::expression,
		"",
		std::move(expression)
	);
}

std::unique_ptr<Expression> Parser::parseExpression() {
	return parseAddition();
}

std::unique_ptr<Expression> Parser::parseAddition() {
	std::unique_ptr<Expression> expression = parseMultiplication();
	while (match(TokenType::plus) || match(TokenType::minus)) {
		TokenType operation = previousToken().type;
		std::unique_ptr<Expression> right = parseMultiplication();
		expression = std::make_unique<TwoExpression>(
			std::move(expression),
			operation,
			std::move(right)
		);
	}
	return expression;
}

std::unique_ptr<Expression> Parser::parseMultiplication() {
	std::unique_ptr<Expression> expression = parseUnary();

	while (match(TokenType::multiply) || match(TokenType::divide)) {
		TokenType operation = previousToken().type;
		std::unique_ptr<Expression> right = parseUnary();
		expression = std::make_unique<TwoExpression>(
			std::move(expression),
			operation,
			std::move(right)
		);
	}
	return expression;
}

std::unique_ptr<Expression> Parser::parseUnary() {
	if (match(TokenType::minus) || match(TokenType::plus)) {
		TokenType operation = previousToken().type;
		std::unique_ptr<Expression> operand = parseUnary();
		return std::make_unique<OneExpression>(
			operation,
			std::move(operand)
		);
	}
	return parsePrimary();
}

std::unique_ptr<Expression> Parser::parsePrimary() {
	if (match(TokenType::number)) {
		return std::make_unique<NumberExpression>(
			previousToken().numberValue
		);
	}
	if (match(TokenType::identifier)) {
		std::string name = previousToken().text;
		if (check(TokenType::left_paren)) {
			return parseFunctionCall(name);
		}
		return std::make_unique<VariableExpression>(name);
	}
	if (match(TokenType::left_paren)) {
		std::unique_ptr<Expression> expression = parseExpression();
		consume(
			TokenType::right_paren,
			"Expected ')' after expression"
		);
		return expression;
	}
	throw std::runtime_error(
		"Expected number, variable, function or '('"
	);
}

std::unique_ptr<Expression> Parser::parseFunctionCall(const std::string& functionName) {
	consume(
		TokenType::left_paren,
		"Expected '(' after function name"
	);
	std::vector<std::unique_ptr<Expression>> arguments;
	if (!check(TokenType::right_paren)) {
		arguments.push_back(parseExpression());
		while (match(TokenType::comma)) {
			arguments.push_back(parseExpression());
		}
	}
	consume(
		TokenType::right_paren,
		"Expected ')' after function arguments"
	);
	return std::make_unique<FunctionCallExpression>(
		functionName,
		std::move(arguments)
	);
}