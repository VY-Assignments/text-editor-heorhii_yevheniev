#include "TextDocument.h"
#include <iostream>
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

TextDocument::~TextDocument(){
	for (int i = 0; i < lines_count; i++) {
		delete lines[i];
		lines[i] = nullptr;
	}
	delete[] lines;
	lines = nullptr;
}