#include "io.h"
#include <stdio.h>


state_t load_program(engine_t *eng, const char *filename)
{
	FILE *file;
	long file_size;
	size_t bytes_read;

	if (!eng || !filename)
		return (SIM_ERR_INVALID_OPCODE);

	file = fopen(filename, "rb");
	if (file == NULL)
	{
		return (SIM_ERR_INVALID_OPCODE);
	}

	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	rewind(file);

	if (file_size > (long)eng->memory.size)
	{
		fclose(file);
		return (SIM_ERR_MEM_OUT_OF_BOUNDS);
	}

	bytes_read = fread(eng->memory.mem, 1, file_size, file);
	
	if (bytes_read != (size_t)file_size)
	{
		fclose(file);
		return (SIM_ERR_INVALID_OPCODE);
	}

	fclose(file);
	return (SIM_SUCCESS);
}
