#include "../../cpu.h"

/**
 * exec_sw - Executes the I-type 'sw' (Store Word) instruction
 * @cpu: Pointer to the CPU state
 * @memory: Pointer to the simulated main memory
 * @inst: Pointer to the decoded instruction
 *
 * Description: Stores a 32-bit word from register rt into memory.
 * Return: SIM_SUCCESS or SIM_ERR_MEM_OUT_OF_BOUNDS
 */
state_t exec_sw(cpu_t *cpu, memory_t *memory, instruction_t *inst)
{
	uint32_t address;
	uint32_t value;

	address = cpu->g_registers[inst->rs] + (int32_t)(int16_t)inst->immediate;
	value = cpu->g_registers[inst->rt];

	if (address + 3 >= memory->size)
		return (SIM_ERR_MEM_OUT_OF_BOUNDS);

	memory->mem[address] = (value >> 24) & 0xFF;
	memory->mem[address + 1] = (value >> 16) & 0xFF;
	memory->mem[address + 2] = (value >> 8) & 0xFF;
	memory->mem[address + 3] = value & 0xFF;

	return (SIM_SUCCESS);
}
