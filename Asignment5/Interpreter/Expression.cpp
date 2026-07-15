#include "Expression.h"
#include "Environment.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>

NumberExpression::NumberExpression(double numberValue)
	:value(numberValue) {
}

double NumberExpression::evaluate(Environment&) const {
	return value;
}

VariableExpression::VariableExpression(const std::string& variableName)
	:name(variableName) {
}

double VariableExpression::evaluate(Environment& environment) const {
	return environment.getVariable(name);
}

OneExpression::OneExpression(
	TokenType operationType,
	std::unique_ptr<Expression> expression
)
	:operation(operationType), operand(std::move(expression)) {
}

double OneExpression::evaluate(Environment& environment) const {
	double value = operand->evaluate(environment);
	switch (operation) {
	case TokenType::minus:
		return -value;
	case TokenType::plus:
		return value;
	default:
		throw std::runtime_error("Wrong one expression operation");
	}
}

TwoExpression::TwoExpression(
	std::unique_ptr<Expression> leftExpression,
	TokenType operationType,
	std::unique_ptr<Expression> rightExpression
)
	:left(std::move(leftExpression)),
	operation(operationType),
	right(std::move(rightExpression)) {
}

double TwoExpression::evaluate(Environment& environment) const {
	double leftValue = left->evaluate(environment);
	double rightValue = right->evaluate(environment);
	switch (operation) {
	case TokenType::plus:
		return leftValue + rightValue;
	case TokenType::minus:
		return leftValue - rightValue;
	case TokenType::multiply:
		return leftValue * rightValue;
	case TokenType::divide:
		if (rightValue == 0.0) {
			throw std::runtime_error("Division by zero");
		}
		return leftValue / rightValue;
	default:
		throw std::runtime_error("Wrong two expression operation");
	}
}

FunctionCallExpression::FunctionCallExpression(
	const std::string& name,
	std::vector<std::unique_ptr<Expression>> functionArguments
)
	:functionName(name), arguments(std::move(functionArguments)) {
}

double FunctionCallExpression::evaluate(Environment& environment) const {
	std::vector<double> values;
	for (const std::unique_ptr<Expression>& argument : arguments) {
		values.push_back(argument->evaluate(environment));
	}
	if (functionName == "pow") {
		if (values.size() != 2) {
			throw std::runtime_error("Function pow expects 2 arguments");
		}
		return std::pow(values[0], values[1]);
	}
	if (functionName == "abs") {
		if (values.size() != 1) {
			throw std::runtime_error("Function abs expects 1 argument");
		}
		return std::abs(values[0]);
	}
	if (functionName == "max") {
		if (values.size() != 2) {
			throw std::runtime_error("Function max expects 2 arguments");
		}
		return std::max(values[0], values[1]);
	}
	if (functionName == "min") {
		if (values.size() != 2) {
			throw std::runtime_error("Function min expects 2 arguments");
		}
		return std::min(values[0], values[1]);
	}
	return environment.callFunction(functionName, values);
}