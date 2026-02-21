#include "../isa.h"


state_t exec_lw(cpu_t *cpu, memory_t *memory, instruction_t *inst)
{
    uint32_t address; /*important to be unsigned (the negative will warm up)*/


    address = cpu->g_registers[inst->rs] + (int32_t)(int16_t)inst->immediate;
    if (inst->rt != 0)
    {
        if(address + 3 >= memory->size)
            return SIM_ERR_MEM_OUT_OF_BOUNDS;
        
        cpu->g_registers[inst->rt] = (memory->mem[address] << 24) | 
                                     (memory->mem[address + 1] << 16) | 
                                     (memory->mem[address + 2] << 8) | 
                                     (memory->mem[address + 3]);
    }
    
    return SIM_SUCCESS;
}
