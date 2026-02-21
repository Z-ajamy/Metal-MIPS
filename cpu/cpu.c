#include "cpu.h"
#include <stddef.h>
#include "isa/isa.h"
#include <stdio.h>

static exec_func_t opcode_table[64] = {NULL};
static exec_func_t r_type_table[64] = {NULL};

static state_t exec_halt(cpu_t *cpu, memory_t *memory, instruction_t *inst)
{
	(void)cpu;
	(void)memory;
	(void)inst;
	return (SIM_HALT);
}


instruction_t decode_instruction(uint32_t instruction)
{
	instruction_t ins;

	ins.op = (instruction >> 26) & 0x3F;
	ins.rs = (instruction >> 21) & 0x1F;
	ins.rt = (instruction >> 16) & 0x1F;
	ins.rd = (instruction >> 11) & 0x1F;
	ins.shamt = (instruction >> 6) & 0x1F;
	ins.funct = instruction & 0x3F;
	ins.immediate = instruction & 0xFFFF;
	ins.address = instruction & 0x03FFFFFF;

	return (ins);
}

static state_t route_r_type(cpu_t *cpu, memory_t *memory, instruction_t *inst)
{
	if (r_type_table[inst->funct] != NULL)
		return (r_type_table[inst->funct](cpu, memory, inst));
		
	return (SIM_ERR_INVALID_OPCODE);
}

void cpu_init(void)
{
    opcode_table[0x00] = route_r_type; /*for all R type*/
    r_type_table[0x20] = exec_add; /*the funct value*/

    opcode_table[0x23] = exec_lw;

    opcode_table[0x3F] = exec_halt;

    opcode_table[0x08] = exec_addi;
}

state_t cpu_tick(cpu_t *cpu, memory_t *memory)
{
	instruction_t instruction;
	uint32_t inst;

	if (cpu->pc + 3 >= memory->size)
		return (SIM_ERR_MEM_OUT_OF_BOUNDS);

	inst = (memory->mem[cpu->pc] << 24) |
	       (memory->mem[cpu->pc + 1] << 16) |
	       (memory->mem[cpu->pc + 2] << 8) |
	       (memory->mem[cpu->pc + 3]);

	cpu->pc += 4;

    printf("DEBUG: Fetching instruction at PC 0x%08X: 0x%08X\n", cpu->pc - 4, inst);
	instruction = decode_instruction(inst);

	if (opcode_table[instruction.op] != NULL)
		return (opcode_table[instruction.op](cpu, memory, &instruction));

	return (SIM_ERR_INVALID_OPCODE);
}
