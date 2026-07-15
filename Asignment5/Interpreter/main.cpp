#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "Lexer.h"
#include "Parser.h"
#include "Environment.h"

int main() {
	std::string input;
	Environment environment;

	std::cout << "Interpreter. Enter an expression:" << std::endl;
	while (std::getline(std::cin, input)) {
		if (input == "exit" || input == "quit") {
			break;
		}
		if (input.empty()) {
			continue;
		}
		try {
			Lexer lexer(input);
			std::vector<Token> tokens = lexer.tokenize();
			Parser parser(tokens);
			ParsedStatement statement = parser.parse();
			if (statement.type == StatementType::variable_declaration) {
				double result = statement.expression->evaluate(environment);
				environment.defineVariable(statement.name, result);
			}
			else if (statement.type == StatementType::function_declaration) {
				std::shared_ptr<Expression> body = std::move(statement.expression);
				environment.defineFunction(
					statement.name,
					statement.parameters,
					std::move(body)
				);
			}
			else {
				double result = statement.expression->evaluate(environment);
				std::cout << result << std::endl;
			}
		}
		catch (const std::exception& error) {
			std::cout << "Error: " << error.what() << std::endl;
		}
	}

	return 0;
}