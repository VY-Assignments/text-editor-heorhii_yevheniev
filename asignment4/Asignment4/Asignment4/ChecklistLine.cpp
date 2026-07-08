#include "ChecklistLine.h"
#include "DllCipher.h"
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

bool ChecklistLine::encrypt(DllCipher& cipher, int key) {
    std::string encryptedItem;
    if (!cipher.encryptCaesar(item, key, encryptedItem)) {
        return false;
    }
    item = encryptedItem;
    return true;
}

bool ChecklistLine::decrypt(DllCipher& cipher, int key) {
    std::string decryptedItem;
    if (!cipher.decryptCaesar(item, key, decryptedItem)) {
        return false;
    }
    item = decryptedItem;
    return true;
}

bool ChecklistLine::encryptVigenere(DllCipher& cipher, const std::string& key) {
    std::string encryptedItem;
    if (!cipher.encryptVigenere(item, key, encryptedItem)) {
        return false;
    }
    item = encryptedItem;
    return true;
}

bool ChecklistLine::decryptVigenere(DllCipher& cipher, const std::string& key) {
    std::string decryptedItem;
    if (!cipher.decryptVigenere(item, key, decryptedItem)) {
        return false;
    }
    item = decryptedItem;
    return true;
}