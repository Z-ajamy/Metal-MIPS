#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

typedef struct memory_s
{
	uint8_t *mem;
	uint32_t size;
} memory_t;

#endif
