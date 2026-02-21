#include "../../cpu.h"

/**
 * exec_beq - Executes the I-type 'beq' (Branch on Equal) instruction
 * @cpu: Pointer to the CPU state
 * @memory: Pointer to the main memory (unused)
 * @inst: Pointer to the decoded instruction
 *
 * Description: Branches to PC-relative target if rs == rt.
 * Return: SIM_SUCCESS
 */
state_t exec_beq(cpu_t *cpu, memory_t *memory, instruction_t *inst)
{
	int32_t offset;

	(void)memory;

	if (cpu->g_registers[inst->rs] == cpu->g_registers[inst->rt])
	{
		offset = (int32_t)(int16_t)inst->immediate;
		cpu->pc = cpu->pc + (offset << 2);
	}
	return (SIM_SUCCESS);
}
