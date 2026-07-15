#include "Environment.h"
#include "Expression.h"
#include <stdexcept>
#include <utility>

UserFunction::UserFunction(
	const std::vector<std::string>& functionParameters,
	std::shared_ptr<Expression> functionBody
)
	:parameters(functionParameters), body(std::move(functionBody)) {
}

Environment::Environment(Environment* parentEnvironment)
	:parent(parentEnvironment) {
}

void Environment::defineVariable(const std::string& name, double value) {
	if (variables.find(name) != variables.end()) {
		throw std::runtime_error(
			"Variable '" + name + "' is already defined"
		);
	}
	variables.emplace(name, value);
}

double Environment::getVariable(const std::string& name) const {
	auto variable = variables.find(name);

	if (variable != variables.end()) {
		return variable->second;
	}

	if (parent != nullptr) {
		return parent->getVariable(name);
	}

	throw std::runtime_error(
		"Unknown variable: " + name
	);
}

bool Environment::hasVariable(const std::string& name) const {
	if (variables.find(name) != variables.end()) {
		return true;
	}
	if (parent != nullptr) {
		return parent->hasVariable(name);
	}
	return false;
}

void Environment::defineFunction(
	const std::string& name,
	const std::vector<std::string>& parameters,
	std::shared_ptr<Expression> body
) {
	if (functions.find(name) != functions.end()) {
		throw std::runtime_error(
			"Function '" + name + "' is already defined"
		);
	}
	functions.emplace(
		name,
		UserFunction(parameters, std::move(body))
	);
}

bool Environment::hasFunction(const std::string& name) const {
	if (functions.find(name) != functions.end()) {
		return true;
	}
	if (parent != nullptr) {
		return parent->hasFunction(name);
	}
	return false;
}

double Environment::callFunction(
	const std::string& name,
	const std::vector<double>& arguments
) {
	auto function = functions.find(name);
	if (function == functions.end()) {
		if (parent != nullptr) {
			return parent->callFunction(name, arguments);
		}

		throw std::runtime_error(
			"Unknown function: " + name
		);
	}
	const UserFunction& userFunction = function->second;
	if (arguments.size() != userFunction.parameters.size()) {
		throw std::runtime_error(
			"Function " + name + " expects " +
			std::to_string(userFunction.parameters.size()) +
			" arguments"
		);
	}
	Environment localEnvironment(this);
	for (std::size_t i = 0; i < arguments.size(); i++) {
		localEnvironment.defineVariable(
			userFunction.parameters[i],
			arguments[i]
		);
	}
	return userFunction.body->evaluate(localEnvironment);
}