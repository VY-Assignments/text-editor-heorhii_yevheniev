#include "ContactLine.h"
#include "DllCipher.h"
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

bool ContactLine::encrypt(DllCipher& cipher, int key) {
    std::string encryptedName;
    std::string encryptedSurname;
    std::string encryptedEmail;
    if (!cipher.encryptCaesar(name, key, encryptedName)) {
        return false;
    }
    if (!cipher.encryptCaesar(surname, key, encryptedSurname)) {
        return false;
    }
    if (!cipher.encryptCaesar(email, key, encryptedEmail)) {
        return false;
    }
    name = encryptedName;
    surname = encryptedSurname;
    email = encryptedEmail;
    return true;
}

bool ContactLine::decrypt(DllCipher& cipher, int key) {
    std::string decryptedName;
    std::string decryptedSurname;
    std::string decryptedEmail;
    if (!cipher.decryptCaesar(name, key, decryptedName)) {
        return false;
    }
    if (!cipher.decryptCaesar(surname, key, decryptedSurname)) {
        return false;
    }
    if (!cipher.decryptCaesar(email, key, decryptedEmail)) {
        return false;
    }
    name = decryptedName;
    surname = decryptedSurname;
    email = decryptedEmail;
    return true;
}

bool ContactLine::encryptVigenere(DllCipher& cipher, const std::string& key) {
    std::string encryptedName;
    std::string encryptedSurname;
    std::string encryptedEmail;
    if (!cipher.encryptVigenere(name, key, encryptedName)) {
        return false;
    }
    if (!cipher.encryptVigenere(surname, key, encryptedSurname)) {
        return false;
    }
    if (!cipher.encryptVigenere(email, key, encryptedEmail)) {
        return false;
    }
    name = encryptedName;
    surname = encryptedSurname;
    email = encryptedEmail;
    return true;
}

bool ContactLine::decryptVigenere(DllCipher& cipher, const std::string& key) {
    std::string decryptedName;
    std::string decryptedSurname;
    std::string decryptedEmail;
    if (!cipher.decryptVigenere(name, key, decryptedName)) {
        return false;
    }
    if (!cipher.decryptVigenere(surname, key, decryptedSurname)) {
        return false;
    }
    if (!cipher.decryptVigenere(email, key, decryptedEmail)) {
        return false;
    }
    name = decryptedName;
    surname = decryptedSurname;
    email = decryptedEmail;
    return true;
}