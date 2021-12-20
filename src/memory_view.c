#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "memory_view.h"


struct memory_view mv_from_cstring(const char *s){
	return (struct memory_view){s, strlen(s)};
}


struct memory_view mv_slice(struct memory_view mv, unsigned int start, unsigned int end){
	if(start < mv.size && start <= end){
		if(end > mv.size)
			end = mv.size;

		return (struct memory_view){
			mv.data + start,
			end - start
		};
	}

	return (struct memory_view){0};
}


struct memory_view mv_take_first(struct memory_view mv, size_t n){
	return mv_slice(mv, 0, n);
}


struct memory_view mv_take_last(struct memory_view mv, size_t n){
	if(n > mv.size)
		n = mv.size;

	return mv_slice(mv, mv.size - n, mv.size);
}


struct memory_view mv_drop_first(struct memory_view mv, size_t n){
	return mv_slice(mv, n, mv.size);
}


struct memory_view mv_drop_last(struct memory_view mv, size_t n){
	if(n > mv.size)
		n = mv.size;

	return mv_slice(mv, 0, mv.size - n);
}


int mv_index(struct memory_view mv, unsigned char x){
	int result = -1;

	if(mv.data != NULL){
		void *first_occurence = memchr(mv.data, x, mv.size);

		if(first_occurence == NULL)
			result = -1;
		else
			result = (uint8_t *)first_occurence - mv.data;
	}

	return result;
}


int mv_index_slice(struct memory_view mv, unsigned char x, unsigned int start, unsigned int end){
	int index = mv_index(mv_slice(mv, start, end), x);

	if(index != -1)
		index += start;

	return index;
}


size_t mv_count(struct memory_view mv, unsigned char x){
	unsigned int count = 0;

	if(mv.data != NULL)
		for(int i = 0; i < mv.size; ++i)
			if(mv.data[i] == x)
				++count;

	return count;
}


bool mv_equals(struct memory_view a, struct memory_view b){
	bool result = false;

	if(a.data != NULL && b.data != NULL && a.size == b.size)
		result = (memcmp(a.data, b.data, a.size) == 0);

	return result;
}


bool mv_startswith(struct memory_view mv, struct memory_view prefix){
	return mv_equals(mv_take_first(mv, prefix.size), prefix);
}


bool mv_endswith(struct memory_view mv, struct memory_view suffix){
	return mv_equals(mv_drop_first(mv, mv.size - suffix.size), suffix);
}


unsigned int mv_dump_to_stream(struct memory_view mv, FILE *fp){
	unsigned int result = 0;

	if(mv.data != NULL)
		result = fwrite(mv.data, 1, mv.size, fp);

	return result;
}
