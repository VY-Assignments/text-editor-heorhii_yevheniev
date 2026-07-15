#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>

class Expression;

struct UserFunction {
	std::vector<std::string> parameters;
	std::shared_ptr<Expression> body;

	UserFunction(
		const std::vector<std::string>& functionParameters,
		std::shared_ptr<Expression> functionBody
	);
};

class Environment {
private:
	std::map<std::string, double> variables;
	std::map<std::string, UserFunction> functions;
	Environment* parent;
public:
	explicit Environment(Environment* parentEnvironment = nullptr);
	void defineVariable(const std::string& name, double value);
	double getVariable(const std::string& name) const;
	bool hasVariable(const std::string& name) const;
	void defineFunction(
		const std::string& name,
		const std::vector<std::string>& parameters,
		std::shared_ptr<Expression> body
	);
	bool hasFunction(const std::string& name) const;
	double callFunction(
		const std::string& name,
		const std::vector<double>& arguments
	);
};