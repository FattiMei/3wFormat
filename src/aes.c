#include <string.h>

#include <nettle/aes.h>
#include "aes.h"


struct memory_view aes256_encrypt_ecb(void *buf, const uint8_t *key, struct memory_view mv){
	struct aes256_ctx ctx;

	if(mv.data != NULL){
		// il più piccolo multiplo di 16 minore o uguale di mv.size
		unsigned int x = mv.size - (mv.size % 16);
		struct aes_block padding = pad_pkcs7((struct memory_view){mv.data, mv.size});

		aes256_set_encrypt_key(&ctx, (const uint8_t*) key);
		aes256_encrypt(&ctx, x, (uint8_t *) buf, (const uint8_t *) mv.data);
		aes256_encrypt(&ctx, 16, (uint8_t *) buf + x, (const uint8_t *) &padding);

		return (struct memory_view){buf, x + 16};
	}

	return (struct memory_view){0};
}


struct memory_view aes256_decrypt_ecb(void *buf, const uint8_t *key, struct memory_view mv){
	struct aes256_ctx ctx;

	if(mv.data != NULL){
		aes256_set_decrypt_key(&ctx, (const uint8_t*) key);
		aes256_decrypt(&ctx, mv.size, (uint8_t *) buf, (const uint8_t *) mv.data);
	
		return (struct memory_view){buf, mv.size};
	}

	return (struct memory_view){0};
}


struct aes_block pad_pkcs7(struct memory_view mv){
	struct aes_block result = {0};

	if(mv.data != NULL){
		unsigned char x = mv.size % 16;
		memcpy(result.data, mv.data + mv.size - x, x);
		memset(result.data + x, 16 - x, 16 - x);
	}

	return result;
}


struct memory_view unpad_pkcs7(struct memory_view mv){
	struct memory_view result = {0};

	if(mv.data != NULL && mv.size % 16 == 0){
		struct memory_view last_block = mv_slice_from_offset_to_end(mv, mv.size - 16);
		unsigned char x = last_block.data[15];

		if(x <= 16 && mv_count(last_block, x) == x)
			result = (struct memory_view){mv.data, mv.size - x};
	}

	return result;
}
