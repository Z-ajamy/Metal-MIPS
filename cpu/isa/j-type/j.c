#include "../../cpu.h"

/**
 * exec_j - Executes the J-type 'j' (Jump) instruction
 * @cpu: Pointer to the CPU state
 * @memory: Pointer to the main memory (unused)
 * @inst: Pointer to the decoded instruction
 *
 * Description: Unconditionally jumps to the calculated absolute address.
 * Return: SIM_SUCCESS
 */
state_t exec_j(cpu_t *cpu, memory_t *memory, instruction_t *inst)
{
	(void)memory;
	/* PC is already PC+4, so we keep top 4 bits and append the address */
	cpu->pc = (cpu->pc & 0xF0000000) | (inst->address << 2);
	return (SIM_SUCCESS);
}
