#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <zlib.h>

#include "memory_view.h"
#include "XYZ3W.h"
#include "aes.h"


const unsigned char HEADER_V5_TEMPLATE[] = {
	'3','D','P','F','N','K','G','1','3','W','T','W',
	1,
	5,
	0, 0,
	0, 0, 0, 8,
	0, 0, 0, 0, 0, 0, 0, 0,
	'T','a','g','E','J','2','5','6',
	0, 0, 0, 0,
	0, 0, 0, 68,
	0, 0, 0, 1,
	0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


// Offset e lunghezza dei blocchi
const uint32_t HEADER_LEN 		= 112;
const uint32_t PRINT_INFO_LEN_OFFSET 	= 36;
const uint32_t CRC32_OFFSET 		= 48;
const uint32_t GCODE_OFFSET 		= 0x2000;


const char *END_STRING = ";END";
const char *NEWLINE = "\r\n";


// Chiave crittografica per AES-256 ECB
const unsigned char *KEY = (unsigned char *)"@xyzprinting.com@xyzprinting.com";


uint32_t ZERO_PADDING_LEN(uint32_t print_info_len){
	return GCODE_OFFSET - HEADER_LEN - print_info_len;
}


struct memory_view pack_header(void *buf, unsigned int print_info_len, unsigned int crc32){
	if(buf != NULL){
		memcpy(buf, HEADER_V5_TEMPLATE, HEADER_LEN);

		#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		print_info_len 	= __builtin_bswap32(print_info_len);
		crc32		= __builtin_bswap32(crc32);
		#endif

		*((uint32_t*) (buf + PRINT_INFO_LEN_OFFSET)) = print_info_len;
		*((uint32_t*) (buf + CRC32_OFFSET)) = crc32;

		return (struct memory_view){buf, HEADER_LEN};
	}

	return (struct memory_view){0};
}


struct memory_view pack_print_info(void *buf, unsigned int print_info_len){
	if(buf != NULL){
		memset(buf, 'M', print_info_len);
		return (struct memory_view){buf, print_info_len};
	}

	return (struct memory_view){0};
}


struct memory_view pack_zero_padding(void *buf, unsigned int print_info_len){
	unsigned int len = ZERO_PADDING_LEN(print_info_len);

	if(buf != NULL){
		memset(buf, 0, len);
		return (struct memory_view){buf, len};
	}

	return (struct memory_view){0};
}


struct memory_view pack_gcode(void *buf, struct memory_view plain_gcode){
	// si suppone che il gcode sia canonico

	if(plain_gcode.data != NULL && buf != NULL){
		return aes256_encrypt_ecb(buf, KEY, plain_gcode);
	}
	
	return (struct memory_view){0};
}


struct xyz_file decompose_3w_file(struct memory_view mv){
	struct xyz_file result = {0};
	struct memory_view 	header,
		  		print_info,
		  		zero_padding,
		  		gcode;

	if(mv.data != NULL){
		int first_zero_offset = mv_index_slice(mv, 0, HEADER_LEN, GCODE_OFFSET);
		if(first_zero_offset < 0)
			first_zero_offset = GCODE_OFFSET;

		header 		= mv_slice(mv, 0, HEADER_LEN);
		print_info 	= mv_slice(mv, HEADER_LEN, first_zero_offset);
		zero_padding 	= mv_slice(mv, HEADER_LEN + print_info.size, GCODE_OFFSET);
		gcode 		= mv_slice(mv, GCODE_OFFSET, mv.size);

		result = (struct xyz_file){header, print_info, zero_padding, gcode};
	}

	return result;
}


bool validate_header(struct memory_view mv){
	bool result = false;

	if(mv.data != NULL){
		if(mv.size == HEADER_LEN){
			struct header_info info = unpack_header(mv);
			unsigned char buf[HEADER_LEN];

			result = mv_equals(	mv,
						pack_header(buf, info.print_info_len, info.crc32));
		}
	}

	return result;
}


bool validate_print_info(struct memory_view mv, uint32_t print_info_len){
	bool result = false;

	if(mv.data != NULL)
		if(mv.size == print_info_len)
			result = (mv_index(mv, 0) == -1);

	return result;
}


bool validate_zero_padding(struct memory_view mv, uint32_t print_info_len){
	bool result = false;

	if(mv.data != NULL)
		if(mv.size == ZERO_PADDING_LEN(print_info_len))
			result = (mv_count(mv, 0) == mv.size);

	return result;
}


bool validate_gcode(struct memory_view mv, uint32_t expected_crc32){
	bool result = false;

	if(mv.data != NULL){
		result = (mv.size % 16 == 0) && (expected_crc32 == crc32(0, mv.data, mv.size));
	}

	return result;
}


bool validate_3w_file(struct xyz_file blocks){
	bool result = false;

	if(validate_header(blocks.header)){
		struct header_info info = unpack_header(blocks.header);

		result = validate_print_info	(blocks.print_info, info.print_info_len) &&
			 validate_zero_padding	(blocks.zero_padding, info.print_info_len) && 
			 validate_gcode		(blocks.gcode, info.crc32);
	}

	return result;
}


struct header_info unpack_header(struct memory_view mv){
	struct header_info result = {0};

	if(mv.data != NULL){
		if(mv.size == HEADER_LEN){
			uint32_t print_info_len = *((uint32_t*) (mv.data + PRINT_INFO_LEN_OFFSET));
			uint32_t crc32		= *((uint32_t*) (mv.data + CRC32_OFFSET));

			#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
			print_info_len 	= __builtin_bswap32(print_info_len);
			crc32		= __builtin_bswap32(crc32);
			#endif

			result = (struct header_info) {print_info_len, crc32};
		}
	}

	return result;
}


struct memory_view unpack_gcode(void *buf, struct memory_view gcode){
	if(gcode.data != NULL && gcode.size > 0 && gcode.size % 16 == 0){
		struct memory_view plain_gcode = aes256_decrypt_ecb(buf, KEY, gcode);

		if(!mv_endswith(plain_gcode, mv_from_string(END_STRING))){
			plain_gcode = unpad_pkcs7(plain_gcode);
		}

		return plain_gcode;
	}

	return (struct memory_view){0};
}


struct memory_view convert_3w_to_gcode(void *buf, struct xyz_file blocks){
	if(validate_3w_file(blocks)){
		return unpack_gcode(buf, blocks.gcode);
	}

	return (struct memory_view){0};
}


struct xyz_file convert_gcode_to_3w(void *buf, struct memory_view gcode){
	return (struct xyz_file){0};
}
