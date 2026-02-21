#include "engine/engine.h"
#include "io/io.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
	engine_t eng;
	state_t status;
	uint32_t mem_size;

	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s <binary_file> <mem_size>\n", argv[0]);
		return (EXIT_FAILURE);
	}

	mem_size = (uint32_t)atoi(argv[2]);

	/* 1. Initialize Hardware */
	status = engine_init(&eng, mem_size);
	if (status != SIM_SUCCESS)
	{
		fprintf(stderr, "Initialization Failed: %s\n", get_error_str(status));
		return (EXIT_FAILURE);
	}

	/* 2. Load Program */
	status = load_program(&eng, argv[1]);
	if (status != SIM_SUCCESS)
	{
		fprintf(stderr, "Loading Failed: %s\n", get_error_str(status));
		engine_cleanup(&eng);
		return (EXIT_FAILURE);
	}

	printf("Simulation Started: %s (%u bytes RAM)\n", argv[1], mem_size);

	/* 3. Run Simulation */
	status = engine_run(&eng);

	/* 4. Display Results */
	if (status != SIM_SUCCESS)
		fprintf(stderr, "\nSimulation Halted with Error: %s\n", get_error_str(status));
	else
		printf("\nSimulation Completed Successfully.\n");

	dump_state(&eng);

	/* 5. Cleanup */
	engine_cleanup(&eng);
	return (status == SIM_SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE);
}
