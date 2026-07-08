#include "TextLine.h"
#include "DllCipher.h"
#include <iostream>


TextLine::TextLine(const std::string& value) {
    text = value;
}

void TextLine::print() const {
    std::cout << "Text: " << text << std::endl;
}

std::string TextLine::serialize() const {
    return "TEXT|" + text;
}

void TextLine::append(const std::string& value) {
    text += value;
}

bool TextLine::insertText(int position, const std::string& value) {
    int length = static_cast<int>(text.length());
    if (position < 0 || position > length) {
        return false;
    }
    text.insert(position, value);
    return true;
}

bool TextLine::replaceText(int position, const std::string& value) {
    int length = static_cast<int>(text.length());
    if (position < 0 || position > length) {
        return false;
    }
    text.replace(position, value.length(), value);
    return true;
}

bool TextLine::deleteText(int position, int count) {
    int length = static_cast<int>(text.length());
    if (position < 0 || count < 0 || position >= length) {
        return false;
    }
    text.erase(position, count);
    return true;
}

bool TextLine::getSubstring(int position, int count, std::string& result) const {
    int length = static_cast<int>(text.length());
    if (position < 0 || count < 0 || position >= length) {
        return false;
    }
    result = text.substr(position, count);
    return true;
}

const std::string& TextLine::getText() const {
    return text;
}

bool TextLine::encrypt(DllCipher& cipher, int key) {
    std::string result;
    if (!cipher.encryptCaesar(text, key, result)) {
        return false;
    }
    text = result;
    return true;
}

bool TextLine::decrypt(DllCipher& cipher, int key) {
    std::string result;
    if (!cipher.decryptCaesar(text, key, result)) {
        return false;
    }
    text = result;
    return true;
}

bool TextLine::encryptVigenere(DllCipher& cipher, const std::string& key) {
    std::string result;
    if (!cipher.encryptVigenere(text, key, result)) {
        return false;
    }
    text = result;
    return true;
}

bool TextLine::decryptVigenere(DllCipher& cipher, const std::string& key) {
    std::string result;
    if (!cipher.decryptVigenere(text, key, result)) {
        return false;
    }
    text = result;
    return true;
}