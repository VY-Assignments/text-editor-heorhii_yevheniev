#include "ChecklistLine.h"
#include <iostream>
ChecklistLine::ChecklistLine(const std::string& item, bool checked) {
	this->item = item;
	this->checked = checked;
}

void ChecklistLine::print() const {
	std::cout << "[ " << (this->checked ? "x" : " ") << " ] "
		<< this->item << std::endl;
}

std::string ChecklistLine::serialize() const {
	return "CHECK|" + std::string(checked ? "1" : "0") + "|" + this->item;
}

void ChecklistLine::toggle() {
	checked = !checked;
}
