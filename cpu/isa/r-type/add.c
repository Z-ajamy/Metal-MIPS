#include "../../cpu.h"

state_t exec_add(cpu_t *cpu, memory_t *memory, instruction_t *inst)
{
    (void)memory; /* Silence unused parameter warning */
    if (inst->rd != 0)
    {
        cpu->g_registers[inst->rd] = cpu->g_registers[inst->rt] + cpu->g_registers[inst->rs];
    }
    
    return SIM_SUCCESS;
}
