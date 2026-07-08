#pragma once
#include "Line.h"
#include <string>

class ContactLine : public Line {
private:
	std::string name;
	std::string surname;
	std::string email;
public:
	ContactLine(const std::string& name,
		const std::string& surname,
		const std::string& email);
	void print() const override;
	std::string serialize() const override;
	bool encrypt(DllCipher& cipher, int key) override;
	bool decrypt(DllCipher& cipher, int key) override;
	bool encryptVigenere(DllCipher& cipher, const std::string& key) override;
	bool decryptVigenere(DllCipher& cipher, const std::string& key) override;
};