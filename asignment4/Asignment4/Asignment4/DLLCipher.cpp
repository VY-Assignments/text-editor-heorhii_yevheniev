#include "DllCipher.h"

DllCipher::DllCipher() {
    library = nullptr;
    createCaesar = nullptr;
    createVigenere = nullptr;
    encryptFunction = nullptr;
    decryptFunction = nullptr;
    destroyFunction = nullptr;
    freeFunction = nullptr;
}

bool DllCipher::load(const std::string& dllPath) {
    unload();
    library = LoadLibraryA(dllPath.c_str());
    if (library == nullptr) {
        return false;
    }
    createCaesar = reinterpret_cast<CreateCaesarFunction>(
        GetProcAddress(library, "cipher_create_caesar")
        );
    createVigenere = reinterpret_cast<CreateVigenereFunction>(
        GetProcAddress(library, "cipher_create_vigenere")
        );
    encryptFunction = reinterpret_cast<CryptFunction>(
        GetProcAddress(library, "cipher_encrypt")
        );
    decryptFunction = reinterpret_cast<CryptFunction>(
        GetProcAddress(library, "cipher_decrypt")
        );
    destroyFunction = reinterpret_cast<DestroyFunction>(
        GetProcAddress(library, "cipher_destroy")
        );
    freeFunction = reinterpret_cast<FreeFunction>(
        GetProcAddress(library, "cipher_free")
        );
    if (createCaesar == nullptr ||
        createVigenere == nullptr ||
        encryptFunction == nullptr ||
        decryptFunction == nullptr ||
        destroyFunction == nullptr ||
        freeFunction == nullptr) {
        unload();
        return false;
    }
    return true;
}

bool DllCipher::isLoaded() const {
    return library != nullptr;
}

void DllCipher::unload() {
    if (library != nullptr) {
        FreeLibrary(library);
        library = nullptr;
    }
    createCaesar = nullptr;
    createVigenere = nullptr;
    encryptFunction = nullptr;
    decryptFunction = nullptr;
    destroyFunction = nullptr;
    freeFunction = nullptr;
}

bool DllCipher::process(
    cipher_t cipher,
    const std::string& input,
    std::string& output,
    bool encryptMode
) {
    if (!isLoaded() || cipher == nullptr) {
        return false;
    }
    char* result = nullptr;
    if (encryptMode) {
        result = encryptFunction(cipher, input.c_str());
    }
    else {
        result = decryptFunction(cipher, input.c_str());
    }

    if (result == nullptr) {
        destroyFunction(cipher);
        return false;
    }
    output = result;
    freeFunction(result);
    destroyFunction(cipher);
    return true;
}

bool DllCipher::encryptCaesar(
    const std::string& plainText,
    int key,
    std::string& encryptedText
) {
    if (!isLoaded()) {
        return false;
    }

    cipher_t cipher = createCaesar(key);
    return process(cipher, plainText, encryptedText, true);
}

bool DllCipher::decryptCaesar(
    const std::string& encryptedText,
    int key,
    std::string& plainText
) {
    if (!isLoaded()) {
        return false;
    }
    cipher_t cipher = createCaesar(key);
    return process(cipher, encryptedText, plainText, false);
}

bool DllCipher::encryptVigenere(
    const std::string& plainText,
    const std::string& key,
    std::string& encryptedText
) {
    if (!isLoaded()) {
        return false;
    }
    cipher_t cipher = createVigenere(key.c_str());
    return process(cipher, plainText, encryptedText, true);
}

bool DllCipher::decryptVigenere(
    const std::string& encryptedText,
    const std::string& key,
    std::string& plainText
) {
    if (!isLoaded()) {
        return false;
    }
    cipher_t cipher = createVigenere(key.c_str());
    return process(cipher, encryptedText, plainText, false);
}

DllCipher::~DllCipher() {
    unload();
}