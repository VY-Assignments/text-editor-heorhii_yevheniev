#include "TextDocument.h"
#include "TextLine.h"
#include "ContactLine.h"
#include "ChecklistLine.h"
#include <iostream>
#include <sstream>
#include <string>
TextDocument::TextDocument() {
	lines_count = 0;
	capacity = 2;
	lines = new Line * [capacity];

	for (int i = 0; i < capacity; i++) {
		lines[i] = nullptr;
	}
}

void TextDocument::resize() {
	int new_capacity = capacity * 2;
	Line** new_lines = new Line * [new_capacity];

	for (int i = 0; i < new_capacity; i++) {
		new_lines[i] = nullptr;
	}

	for (int i = 0; i < lines_count; i++) {
		new_lines[i] = lines[i];
	}

	delete[] lines;

	lines = new_lines;
	capacity = new_capacity;
}

void TextDocument::addLine(Line* line) {
	if (line == nullptr) {
		return;
	}
	if (lines_count >= capacity) {
		resize();
	}
	lines[lines_count] = line;
	lines_count++;
}

void TextDocument::print() const {
	if (lines_count == 0) {
		std::cout << "file is empty" << std::endl;
		return;
	}

	for (int i = 0;i < lines_count; i++) {
		std::cout << i << ": ";
		lines[i]->print();
	}
}

std::string TextDocument::serialize() const {
	std::string result;
	for (int i = 0; i < lines_count; i++) {
		result += lines[i]->serialize();
		if (i < lines_count - 1) {
			result += "\n";
		}
	}
	return result;
}

int TextDocument::getLineCount() const {
	return lines_count;
}

Line* TextDocument::getLine(int index) const {
	if (index < 0 || index >= lines_count) {
		return nullptr;
	}
	return lines[index];
}

void TextDocument::clear() {
	for (int i = 0; i < lines_count; i++) {
		delete lines[i];
		lines[i] = nullptr;
	}
	lines_count = 0;
}

bool TextDocument::deserialize(const std::string& data) {
	clear();
	std::stringstream stream(data);
	std::string line;
	while (std::getline(stream, line)) {
		if (line.empty()) {
			continue;
		}

		if (line.rfind("TEXT|", 0) == 0) {
			std::string text = line.substr(5);
			addLine(new TextLine(text));
		}
		else if (line.rfind("CHECK|", 0) == 0) {
			size_t firstSeparator = line.find('|');
			size_t secondSeparator = line.find('|', firstSeparator + 1);

			if (secondSeparator == std::string::npos) {
				return false;
			}
			std::string checkedText = line.substr(firstSeparator + 1, secondSeparator - firstSeparator - 1);
			std::string item = line.substr(secondSeparator + 1);

			bool checked = false;
			if (checkedText == "1") {
				checked = true;
			}else if (checkedText == "0") {
				checked = false;
			}else {
				return false;
			}
			addLine(new ChecklistLine(item, checked));
		}
		else if (line.rfind("CONTACT|", 0) == 0) {
			size_t firstSeparator = line.find('|');
			size_t secondSeparator = line.find('|', firstSeparator + 1);
			size_t thirdSeparator = line.find('|', secondSeparator + 1);

			if (secondSeparator == std::string::npos || thirdSeparator == std::string::npos) {
				return false;
			}

			std::string name = line.substr(firstSeparator + 1, secondSeparator - firstSeparator - 1);
			std::string surname = line.substr(secondSeparator + 1, thirdSeparator - secondSeparator - 1);
			std::string email = line.substr(thirdSeparator + 1);

			addLine(new ContactLine(name, surname, email));
		}
		else {
			return false;
		}
	}
	return true;
}

TextDocument::~TextDocument(){
	for (int i = 0; i < lines_count; i++) {
		delete lines[i];
		lines[i] = nullptr;
	}
	delete[] lines;
	lines = nullptr;
}