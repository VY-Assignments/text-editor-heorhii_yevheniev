#include "pch.h"

#define CIPHER_DLL_EXPORTS
#include "cipher_api.h"
#include <string>
#include <cstring>
#include <cstddef>

static bool is_uppercase_letter(char ch) {
	return ch >= 'A' && ch <= 'Z';
}

static bool is_lowercase_letter(char ch) {
	return ch >= 'a' && ch <= 'z';
}

static bool is_letter(char ch) {
	return is_uppercase_letter(ch) || is_lowercase_letter(ch);
}

static int normalize_shift(int shift) {
	shift = shift % 26;

	if (shift < 0) {
		shift += 26;
	}
	return shift;
}

static char shift_char(char ch, int shift) {
	shift = normalize_shift(shift);
	if (is_uppercase_letter(ch)) {
		return static_cast<char>('A' + (ch - 'A' + shift) % 26);
	}

	if (is_lowercase_letter(ch)) {
		return static_cast<char>('a' + (ch - 'a' + shift) % 26);
	}
	return ch;
}

static int key_letter_to_shift(char keyChar) {
	if (is_uppercase_letter(keyChar)) {
		return keyChar - 'A';
	}
	if (is_lowercase_letter(keyChar)) {
		return keyChar - 'a';
	}
	return 0;
}

static char* copy_string_to_c_string(const std::string& text) {
	char* result = new char[text.size() + 1];
	std::memcpy(result, text.c_str(), text.size() + 1);
	return result;
}


class Cipher {
public:
	virtual std::string encrypt(const std::string& text) = 0;
	virtual std::string decrypt(const std::string& text) = 0;
	virtual ~Cipher() = default;
};

class CaesarCipher : public Cipher {
private:
	int key_;

public:
	CaesarCipher(int key) {
		key_ = key;
	}
	std::string encrypt(const std::string& text) override {
		std::string result = text;
		for (std::size_t i = 0; i < result.size(); i++) {
			result[i] = shift_char(result[i], key_);
		}
		return result;
	}
	std::string decrypt(const std::string& text) override {
		std::string result = text;
		for (std::size_t i = 0; i < result.size(); i++) {
			result[i] = shift_char(result[i], -key_);
		}
		return result;
	}
};

class VigenereCipher : public Cipher {
private:
	std::string key_;
public:
	VigenereCipher(const std::string& key) {
		for (std::size_t i = 0; i < key.size(); i++) {
			if (is_letter(key[i])) {
				key_ += key[i];
			}
		}

		if (key_.empty()) {
			key_ = "A";
		}
	}
	std::string encrypt(const std::string& text) override {
		return process(text, true);
	}
	std::string decrypt(const std::string& text) override {
		return process(text, false);
	}
private:
	std::string process(const std::string& text, bool encryptMode) {
		std::string result = text;
		std::size_t keyIndex = 0;
		for (std::size_t i = 0; i < result.size(); i++) {
			if (is_letter(result[i])) {
				char keyChar = key_[keyIndex % key_.size()];
				int shift = key_letter_to_shift(keyChar);

				if (encryptMode == false) {
					shift = -shift;
				}
				result[i] = shift_char(result[i], shift);
				keyIndex++;
			}
		}
		return result;
	}
};


extern "C" {
	CIPHER_API cipher_t cipher_create_caesar(int key) {
		Cipher* cipher = new CaesarCipher(key);
		return static_cast<cipher_t>(cipher);
	}
	CIPHER_API cipher_t cipher_create_vigenere(const char* key) {
		if (key == nullptr) {
			return nullptr;
		}
		Cipher* cipher = new VigenereCipher(key);
		return static_cast<cipher_t>(cipher);
	}
	CIPHER_API char* cipher_encrypt(cipher_t cipher, const char* text) {
		if (cipher == nullptr || text == nullptr) {
			return nullptr;
		}
		Cipher* realCipher = static_cast<Cipher*>(cipher);
		std::string encryptedText = realCipher->encrypt(text);
		return copy_string_to_c_string(encryptedText);
	}
	CIPHER_API char* cipher_decrypt(cipher_t cipher, const char* text) {
		if (cipher == nullptr || text == nullptr) {
			return nullptr;
		}

		Cipher* realCipher = static_cast<Cipher*>(cipher);
		std::string decryptedText = realCipher->decrypt(text);
		return copy_string_to_c_string(decryptedText);
	}
	CIPHER_API void cipher_destroy(cipher_t cipher) {
		Cipher* realCipher = static_cast<Cipher*>(cipher);
		delete realCipher;
	}
	CIPHER_API void cipher_free(char* str) {
		delete[] str;
	}
}
