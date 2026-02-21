#include "io.h"


const char *get_error_str(state_t status)
{
	switch (status)
	{
		case SIM_ERR_INVALID_OPCODE:
			return ("Invalid Opcode: The CPU encountered an instruction it doesn't recognize.\n"
				"Check if your binary contains unsupported instructions.");
		case SIM_ERR_MEM_OUT_OF_BOUNDS:
			return ("Memory Out of Bounds: The program tried to access RAM outside its limit.\n"
				"Possible causes: Array overflow, invalid pointer, or PC ran past the code.");
		case SIM_ERR_MALLOC:
			return ("Malloc Error: Failed to allocate physical RAM for the simulator.");
		default:
			return ("Unknown Error: An unexpected system failure occurred.");
	}
}


void dump_state(engine_t *eng)
{
	int i;

	printf("\n--- CPU STATE DUMP ---\n");
	printf("PC: 0x%08X\n", eng->cpu.pc);
	printf("Registers:\n");
	for (i = 0; i < 32; i++)
	{
		printf("$%02d: 0x%08X  ", i, eng->cpu.g_registers[i]);
		if ((i + 1) % 4 == 0) printf("\n");
	}
	printf("----------------------\n");
}
