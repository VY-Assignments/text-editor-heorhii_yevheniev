#include "ContactLine.h"
#include <iostream>

ContactLine::ContactLine(const std::string& name,
	const std::string& surname,
	const std::string& email) {
	this->name = name;
	this->surname = surname;
	this->email = email;
}

void ContactLine::print() const {
	std::cout << "Contact - " << this->name << " " << this->surname 
		<< ", E-mail: " << this->email << std::endl;
}
std::string ContactLine::serialize() const {
	return "CONTACT|" + this->name + "|" + this->surname + "|" + this->email;
}