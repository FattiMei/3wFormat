#ifndef _AES_H_
#define _AES_H_


#include <stdint.h>
#include "memory_view.h"


struct aes_block{
	uint8_t data[16];
};


// funzioni wrapper della libreria nettle, solo ad uso interno
// estremamete unsafe, guardare la documentazione

struct memory_view aes256_encrypt_ecb(void*, const uint8_t*, struct memory_view);
struct memory_view aes256_decrypt_ecb(void*, const uint8_t*, struct memory_view);


// funzioni per generare o togliere il padding PKCS#7
struct aes_block pad_pkcs7(struct memory_view);
struct memory_view unpad_pkcs7(struct memory_view);


#endif
