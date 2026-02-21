#include "../../cpu.h"

/**
 * exec_sub - Executes the R-type 'sub' instruction
 * @cpu: Pointer to the CPU state
 * @memory: Pointer to the main memory (unused)
 * @inst: Pointer to the decoded instruction
 *
 * Description: Subtracts register rt from rs, stores result in rd.
 * Return: SIM_SUCCESS
 */
state_t exec_sub(cpu_t *cpu, memory_t *memory, instruction_t *inst)
{
	(void)memory;

	if (inst->rd != 0)
	{
		cpu->g_registers[inst->rd] = cpu->g_registers[inst->rs] -
					     cpu->g_registers[inst->rt];
	}
	return (SIM_SUCCESS);
}
