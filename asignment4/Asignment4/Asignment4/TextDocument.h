#pragma once
#include "Line.h"
#include <string>

class TextDocument {
private:
	Line** lines;
	int lines_count;
	int capacity;
	void resize();
public:
	TextDocument();
	TextDocument(const TextDocument& other) = delete;
	TextDocument& operator=(const TextDocument& other) = delete;

	void addLine(Line* line);
	void print() const;
	std::string serialize() const;

	int getLineCount() const;
	Line* getLine(int index) const;
	~TextDocument();
};