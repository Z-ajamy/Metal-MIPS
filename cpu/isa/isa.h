#ifndef ISA_H
#define ISA_H

#include "../cpu.h"

state_t exec_add(cpu_t *cpu, memory_t *memory, instruction_t *inst);
state_t exec_lw(cpu_t *cpu, memory_t *memory, instruction_t *inst);
state_t exec_addi(cpu_t *cpu, memory_t *memory, instruction_t *inst);
state_t exec_sub(cpu_t *cpu, memory_t *memory, instruction_t *inst);
state_t exec_slt(cpu_t *cpu, memory_t *memory, instruction_t *inst);
state_t exec_sw(cpu_t *cpu, memory_t *memory, instruction_t *inst);
state_t exec_beq(cpu_t *cpu, memory_t *memory, instruction_t *inst);
state_t exec_j(cpu_t *cpu, memory_t *memory, instruction_t *inst);

#endif
