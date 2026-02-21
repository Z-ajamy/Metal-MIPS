#include "../../cpu.h"

/**
 * exec_slt - Executes the R-type 'slt' (Set on Less Than) instruction
 * @cpu: Pointer to the CPU state
 * @memory: Pointer to the main memory (unused)
 * @inst: Pointer to the decoded instruction
 *
 * Description: If rs < rt (signed), rd = 1; else rd = 0.
 * Return: SIM_SUCCESS
 */
state_t exec_slt(cpu_t *cpu, memory_t *memory, instruction_t *inst)
{
	(void)memory;

	if (inst->rd != 0)
	{
		if ((int32_t)cpu->g_registers[inst->rs] < (int32_t)cpu->g_registers[inst->rt])
			cpu->g_registers[inst->rd] = 1;
		else
			cpu->g_registers[inst->rd] = 0;
	}
	return (SIM_SUCCESS);
}
