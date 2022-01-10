#ifndef _CRC32_H
#define _CRC32_H


#include <stdint.h>
#include <stddef.h>


uint32_t crc32(uint32_t, const uint8_t*, size_t);


#ifdef _MEMORY_VIEW_H
uint32_t crc32_mv(uint32_t, struct memory_view);
#endif


#endif
