#include <iostream>
#include <string>
#include <windows.h>

typedef void* cipher_t;

typedef cipher_t(*CreateCaesarFunc)(int);
typedef cipher_t(*CreateVigenereFunc)(const char*);
typedef char* (*EncryptFunc)(cipher_t, const char*);
typedef char* (*DecryptFunc)(cipher_t, const char*);
typedef void(*DestroyFunc)(cipher_t);
typedef void(*FreeStringFunc)(char*);

int main() {
	HMODULE library = LoadLibraryA("EncriptionModule.dll");

	if (library == nullptr) {
		std::cout << "can not load dll\n";
		return 1;
	}
	CreateCaesarFunc createCaesar = reinterpret_cast<CreateCaesarFunc>(GetProcAddress(library, "cipher_create_caesar"));
	CreateVigenereFunc createVigenere = reinterpret_cast<CreateVigenereFunc>(GetProcAddress(library, "cipher_create_vigenere"));
	EncryptFunc encrypt = reinterpret_cast<EncryptFunc>(GetProcAddress(library, "cipher_encrypt"));
	DecryptFunc decrypt = reinterpret_cast<DecryptFunc>(GetProcAddress(library, "cipher_decrypt"));
	DestroyFunc destroyCipher = reinterpret_cast<DestroyFunc>(GetProcAddress(library, "cipher_destroy"));
	FreeStringFunc freeString = reinterpret_cast<FreeStringFunc>(GetProcAddress(library, "cipher_free"));

	if (createCaesar == nullptr || createVigenere == nullptr ||
		encrypt == nullptr || decrypt == nullptr ||
		destroyCipher == nullptr || freeString == nullptr) {
		std::cout << "Some functions did not load\n";
		FreeLibrary(library);
		return 1;
	}

	int cipherC;
	int operC;

	std::cout << "choose cipher:\n";
	std::cout << "1. Caesar Cipher\n";
	std::cout << "2. Vigenere Cipher\n";
	std::cin >> cipherC;

    std::cout << "what to do?\n";
	std::cout << "1. Encrypt\n";
	std::cout << "2. Decrypt\n";
	std::cin >> operC;

	std::cin.ignore(1000, '\n');
	cipher_t cipher = nullptr;

	if (cipherC == 1) {
		int key;

		std::cout << "\nEnter Caesar key\n";
		std::cin >> key;
		std::cin.ignore(1000, '\n');
		cipher = createCaesar(key);
	}
	else if (cipherC == 2) {
		std::string key;

		std::cout << "\nEnter Vigenere key\n";
		std::getline(std::cin, key);
		cipher = createVigenere(key.c_str());
	}
	else {
		std::cout << "invalid input\n";
		FreeLibrary(library);
		return 1;
	}

	if (cipher == nullptr) {
		std::cout << "unable to create cipher\n";
		FreeLibrary(library);
		return 1;
	}

	std::string text;
	std::cout << "\nenter text\n";
	std::getline(std::cin, text);
	char* result = nullptr;
	if (operC == 1) {
		result = encrypt(cipher, text.c_str());
	}
	else if (operC == 2) {
		result = decrypt(cipher, text.c_str());
	}
	else {
		std::cout << "invalid input(operC)\n";
		destroyCipher(cipher);
		FreeLibrary(library);
		return 1;
	}

	if (result == nullptr) {
		std::cout << "Operation Failed\n";
		destroyCipher(cipher);
		FreeLibrary(library);
		return 1;
	}
	std::cout << "\nResult:\n";
	std::cout << result;
	std::cout << "\n";
	freeString(result);
	destroyCipher(cipher);
	FreeLibrary(library);

	return 0;
}