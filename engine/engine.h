#ifndef ENGINE_H
#define ENGINE_H

#include <stdlib.h>
#include <stdint.h>
#include "../cpu/cpu.h"
#include "../memory/memory.h"

typedef struct engine_s
{
    cpu_t cpu;
    memory_t memory;
    int is_running;
} engine_t;

state_t engine_init(engine_t *eng, uint32_t mem_size);
state_t engine_run(engine_t *eng);
void engine_cleanup(engine_t *engine);

#endif
