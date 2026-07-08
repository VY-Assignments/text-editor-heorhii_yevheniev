#pragma once
#include <string>
class DllCipher;
class Line {
public:
    virtual void print() const = 0;
    virtual std::string serialize() const = 0;
    virtual bool encrypt(DllCipher& cipher, int key) = 0;
    virtual bool decrypt(DllCipher& cipher, int key) = 0;
    virtual bool encryptVigenere(DllCipher& cipher, const std::string& key) = 0;
    virtual bool decryptVigenere(DllCipher& cipher, const std::string& key) = 0;
    virtual ~Line() {}
};