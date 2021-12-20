#ifndef _MEMORY_VIEW_H
#define _MEMORY_VIEW_H


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


#define MV(ptr, size) (struct memory_view){(ptr), (size)}
#define MV_NULL MV(NULL, 0)


struct memory_view{
	const uint8_t *data;
	size_t size;
};


// Costruttori
const struct memory_view mv_from_cstring(const char *);


struct memory_view mv_slice(struct memory_view, unsigned int, unsigned int);

struct memory_view mv_take_first(struct memory_view, size_t);
struct memory_view mv_take_last	(struct memory_view, size_t);
struct memory_view mv_drop_first(struct memory_view, size_t);
struct memory_view mv_drop_last	(struct memory_view, size_t);


int mv_index(struct memory_view, unsigned char);
int mv_index_slice(struct memory_view, unsigned char, unsigned int, unsigned int);


size_t mv_count(struct memory_view, unsigned char);


bool mv_equals(struct memory_view, struct memory_view);
bool mv_startswith(struct memory_view, struct memory_view);
bool mv_endswith(struct memory_view, struct memory_view);


unsigned int mv_dump_to_stream(struct memory_view, FILE *);


#endif
