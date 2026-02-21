#ifndef IO_H
#define IO_H

#include "../engine/engine.h"
#include <stdio.h>


state_t load_program(engine_t *eng, const char *filename);

void dump_state(engine_t *eng);
const char *get_error_str(state_t status);
#endif
