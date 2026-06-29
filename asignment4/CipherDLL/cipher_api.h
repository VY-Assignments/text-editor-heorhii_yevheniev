#pragma once

#ifdef _WIN32
#if defined(CIPHER_DLL_EXPORTS) || defined(ENCRIPTIONMODULE_EXPORTS) || defined(ENCRYPTIONMODULE_EXPORTS)
#define CIPHER_API __declspec(dllexport)
#else
#define CIPHER_API __declspec(dllimport)
#endif
#else
#define CIPHER_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

    typedef void* cipher_t;

	CIPHER_API cipher_t cipher_create_caesar(int key);
	CIPHER_API cipher_t cipher_create_vigenere(const char* key);

	CIPHER_API char* cipher_encrypt(cipher_t cipher, const char* text);
	CIPHER_API char* cipher_decrypt(cipher_t cipher, const char* text);

	CIPHER_API void cipher_destroy(cipher_t cipher);
	CIPHER_API void cipher_free(char* str);
#ifdef __cplusplus
}
#endif