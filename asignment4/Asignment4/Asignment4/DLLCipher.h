#pragma once

#include <windows.h>
#include <string>

typedef void* cipher_t;

class DllCipher {
private:
    HMODULE library;
    typedef cipher_t(*CreateCaesarFunction)(int);
    typedef cipher_t(*CreateVigenereFunction)(const char*);
    typedef char* (*CryptFunction)(cipher_t cipher, const char* text);
    typedef void (*DestroyFunction)(cipher_t cipher);
    typedef void (*FreeFunction)(char* text);
    CreateCaesarFunction createCaesar;
    CreateVigenereFunction createVigenere;
    CryptFunction encryptFunction;
    CryptFunction decryptFunction;
    DestroyFunction destroyFunction;
    FreeFunction freeFunction;
    bool process(
        cipher_t cipher,
        const std::string& input,
        std::string& output,
        bool encryptMode
    );
public:
    DllCipher();
    bool load(const std::string& dllPath);
    bool isLoaded() const;
    void unload();
    bool encryptCaesar(
        const std::string& plainText,
        int key,
        std::string& encryptedText
    );
    bool decryptCaesar(
        const std::string& encryptedText,
        int key,
        std::string& plainText
    );
    bool encryptVigenere(
        const std::string& plainText,
        const std::string& key,
        std::string& encryptedText
    );
    bool decryptVigenere(
        const std::string& encryptedText,
        const std::string& key,
        std::string& plainText
    );

    ~DllCipher();
};