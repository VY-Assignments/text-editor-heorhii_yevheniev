#pragma once
#include "Line.h"
#include <string>

class TextLine : public Line {
private:
    std::string text;
public:
    TextLine(const std::string& value);

    void print() const override;
    std::string serialize() const override;
    bool encrypt(DllCipher& cipher, int key) override;
    bool decrypt(DllCipher& cipher, int key) override;
    bool encryptVigenere(DllCipher& cipher, const std::string& key) override;
    bool decryptVigenere(DllCipher& cipher, const std::string& key) override;
    void append(const std::string& value);
    bool insertText(int position, const std::string& value);
    bool replaceText(int position, const std::string& value);
    bool deleteText(int position, int count);
    bool getSubstring(int position, int count, std::string& result) const;

    const std::string& getText() const;
};