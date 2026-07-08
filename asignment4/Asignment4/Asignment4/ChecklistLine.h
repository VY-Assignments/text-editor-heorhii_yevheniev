#pragma once
#include"Line.h"
#include<string>

class ChecklistLine : public Line {
private:
	std::string item;
	bool checked;
public:
	ChecklistLine(const std::string& item, bool checked);
	void print() const override;
	std::string serialize() const override;
	bool encrypt(DllCipher& cipher, int key) override;
	bool decrypt(DllCipher& cipher, int key) override;
	bool encryptVigenere(DllCipher& cipher, const std::string& key) override;
	bool decryptVigenere(DllCipher& cipher, const std::string& key) override;
	void toggle();
};