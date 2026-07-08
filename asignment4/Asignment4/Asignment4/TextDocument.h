#pragma once
#include "Line.h"
#include <string>

class DllCipher;

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
	bool encryptAll(DllCipher& cipher, int key);
	bool decryptAll(DllCipher& cipher, int key);
	bool encryptAllVigenere(DllCipher& cipher, const std::string& key);
	bool decryptAllVigenere(DllCipher& cipher, const std::string& key);
	void clear();
	bool deserialize(const std::string& data);
	~TextDocument();
};