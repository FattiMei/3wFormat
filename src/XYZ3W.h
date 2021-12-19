#ifndef _XYZ3W_H
#define _XYZ3W_H


#include <stdint.h>
#include <stdbool.h>
#include "memory_view.h"


struct header_info{
	unsigned int print_info_len,
		     crc32;
};


struct xyz_file{
	struct memory_view header,
			   print_info,
			   zero_padding,
			   gcode;
};

extern const unsigned char HEADER_V5_TEMPLATE[];
extern const uint32_t HEADER_LEN;


uint32_t ZERO_PADDING_LEN(uint32_t);


struct memory_view pack_header		(void *, unsigned int, unsigned int);
struct memory_view pack_print_info	(void *, unsigned int);
struct memory_view pack_zero_padding	(void *, unsigned int);
struct memory_view pack_gcode		(void *, struct memory_view);


struct xyz_file decompose_3w_file(struct memory_view);


bool validate_header		(struct memory_view);
bool validate_print_info	(struct memory_view, unsigned int);
bool validate_zero_padding	(struct memory_view, unsigned int);
bool validate_gcode		(struct memory_view, unsigned int);
bool validate_3w_file		(struct xyz_file);


struct header_info unpack_header(struct memory_view);
struct memory_view unpack_gcode	(void *, struct memory_view);


struct memory_view convert_3w_to_gcode(void *, struct xyz_file);
struct xyz_file convert_gcode_to_3w(void *, struct memory_view);


#endif
