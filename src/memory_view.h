#ifndef _MEMORY_VIEW_H
#define _MEMORY_VIEW_H


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


struct memory_view{
	const uint8_t *data;
	uint32_t size;
};


// Costruttori
const struct memory_view mv_from_string(const char *);


struct memory_view mv_slice(struct memory_view, unsigned int, unsigned int);
struct memory_view mv_slice_from_offset_to_end(struct memory_view, unsigned int);
struct memory_view mv_slice_from_start_to_offset(struct memory_view, unsigned int);


int mv_index(struct memory_view, unsigned char);
int mv_index_slice(struct memory_view, unsigned char, unsigned int, unsigned int);


unsigned int mv_count(struct memory_view, unsigned char);


bool mv_equals(struct memory_view, struct memory_view);
bool mv_startswith(struct memory_view, struct memory_view);
bool mv_endswith(struct memory_view, struct memory_view);


unsigned int mv_dump_to_stream(struct memory_view, FILE *);
#endif
