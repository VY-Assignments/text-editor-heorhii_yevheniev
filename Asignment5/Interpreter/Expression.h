#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Token.h"

class Environment;

class Expression {
public:
	virtual double evaluate(Environment& environment) const = 0;
	virtual ~Expression() = default;
};

class NumberExpression : public Expression {
private:
	double value;
public:
	explicit NumberExpression(double numberValue);
	double evaluate(Environment& environment) const override;
};

class VariableExpression : public Expression {
private:
	std::string name;
public:
	explicit VariableExpression(const std::string& variableName);
	double evaluate(Environment& environment) const override;
};

class OneExpression : public Expression {
private:
	TokenType operation;
	std::unique_ptr<Expression> operand;
public:
	OneExpression(
		TokenType operationType,
		std::unique_ptr<Expression> expression
	);
	double evaluate(Environment& environment) const override;
};

class TwoExpression : public Expression {
private:
	std::unique_ptr<Expression> left;
	TokenType operation;
	std::unique_ptr<Expression> right;
public:
	TwoExpression(
		std::unique_ptr<Expression> leftExpression,
		TokenType operationType,
		std::unique_ptr<Expression> rightExpression
	);
	double evaluate(Environment& environment) const override;
};

class FunctionCallExpression : public Expression {
private:
	std::string functionName;
	std::vector<std::unique_ptr<Expression>> arguments;
public:
	FunctionCallExpression(
		const std::string& name,
		std::vector<std::unique_ptr<Expression>> functionArguments
	);
	double evaluate(Environment& environment) const override;
};