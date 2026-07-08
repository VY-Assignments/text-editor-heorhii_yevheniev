#include "pch.h"
#define CIPHER_DLL_EXPORTS
#include "cipher_api.h"
#include <string>
#include <cstring>

class Cipher {
public:
    virtual char* encrypt(const char* text) = 0;
    virtual char* decrypt(const char* text) = 0;
    virtual ~Cipher() {}
};

static char* create_result_string(const std::string& text) {
    char* result = new char[text.length() + 1];

    for (size_t i = 0; i < text.length(); i++) {
        result[i] = text[i];
    }

    result[text.length()] = '\0';
    return result;
}

static bool is_uppercase_letter(char symbol) {
    return symbol >= 'A' && symbol <= 'Z';
}

static bool is_lowercase_letter(char symbol) {
    return symbol >= 'a' && symbol <= 'z';
}

static bool is_letter(char symbol) {
    return is_uppercase_letter(symbol) || is_lowercase_letter(symbol);
}

static int normalize_shift(int shift) {
    int result = shift % 26;
    if (result < 0) {
        result += 26;
    }
    return result;
}

static char shift_char(char symbol, int shift) {
    if (!is_letter(symbol)) {
        return symbol;
    }
    int normalizedShift = normalize_shift(shift);
    if (is_uppercase_letter(symbol)) {
        return static_cast<char>('A' + (symbol - 'A' + normalizedShift) % 26);
    }
    return static_cast<char>('a' + (symbol - 'a' + normalizedShift) % 26);
}

static int key_letter_to_shift(char symbol) {
    if (is_uppercase_letter(symbol)) {
        return symbol - 'A';
    }
    if (is_lowercase_letter(symbol)) {
        return symbol - 'a';
    }
    return 0;
}

class CaesarCipher : public Cipher {
private:
    int key;
public:
    CaesarCipher(int keyValue) {
        key = keyValue;
    }

    char* encrypt(const char* text) override {
        if (text == nullptr) {
            return nullptr;
        }
        std::string result;
        for (int i = 0; text[i] != '\0'; i++) {
            result += shift_char(text[i], key);
        }
        return create_result_string(result);
    }

    char* decrypt(const char* text) override {
        if (text == nullptr) {
            return nullptr;
        }
        std::string result;
        for (int i = 0; text[i] != '\0'; i++) {
            result += shift_char(text[i], -key);
        }
        return create_result_string(result);
    }
};

class VigenereCipher : public Cipher {
private:
    std::string key;
public:
    VigenereCipher(const char* keyValue) {
        if (keyValue == nullptr || keyValue[0] == '\0') {
            key = "a";
        }
        else {
            key = keyValue;
        }
    }

    char* encrypt(const char* text) override {
        if (text == nullptr) {
            return nullptr;
        }

        std::string result;
        int keyIndex = 0;

        for (int i = 0; text[i] != '\0'; i++) {
            char current = text[i];

            if (is_letter(current)) {
                int shift = key_letter_to_shift(key[keyIndex % key.length()]);
                result += shift_char(current, shift);
                keyIndex++;
            }
            else {
                result += current;
            }
        }

        return create_result_string(result);
    }

    char* decrypt(const char* text) override {
        if (text == nullptr) {
            return nullptr;
        }

        std::string result;
        int keyIndex = 0;

        for (int i = 0; text[i] != '\0'; i++) {
            char current = text[i];

            if (is_letter(current)) {
                int shift = key_letter_to_shift(key[keyIndex % key.length()]);
                result += shift_char(current, -shift);
                keyIndex++;
            }
            else {
                result += current;
            }
        }

        return create_result_string(result);
    }
};

extern "C" {
    CIPHER_API cipher_t cipher_create_caesar(int key) {
        return new CaesarCipher(key);
    }
    CIPHER_API cipher_t cipher_create_vigenere(const char* key) {
        return new VigenereCipher(key);
    }
    CIPHER_API char* cipher_encrypt(cipher_t cipher, const char* text) {
        if (cipher == nullptr || text == nullptr) {
            return nullptr;
        }

        Cipher* cipherObject = static_cast<Cipher*>(cipher);

        return cipherObject->encrypt(text);
    }
    CIPHER_API char* cipher_decrypt(cipher_t cipher, const char* text) {
        if (cipher == nullptr || text == nullptr) {
            return nullptr;
        }

        Cipher* cipherObject = static_cast<Cipher*>(cipher);

        return cipherObject->decrypt(text);
    }
    CIPHER_API void cipher_destroy(cipher_t cipher) {
        if (cipher == nullptr) {
            return;
        }

        Cipher* cipherObject = static_cast<Cipher*>(cipher);

        delete cipherObject;
    }
    CIPHER_API void cipher_free(char* text) {
        delete[] text;
    }
}