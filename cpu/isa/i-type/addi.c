#include "../isa.h"


state_t exec_addi(cpu_t *cpu, memory_t *memory, instruction_t *inst)
{
    (void)memory;
    if (inst->rt != 0)
        cpu->g_registers[inst->rt] = cpu->g_registers[inst->rs] + (int32_t)(int16_t)inst->immediate;
    
    return SIM_SUCCESS;
}
