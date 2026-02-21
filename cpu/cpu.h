#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "../memory/memory.h"

typedef enum state_e
{
	SIM_SUCCESS = 0,
	SIM_HALT,
	SIM_ERR_INVALID_OPCODE,
	SIM_ERR_MEM_OUT_OF_BOUNDS,
	SIM_ERR_MALLOC
} state_t;

typedef struct instruction_s
{
	uint8_t op;
	uint8_t rs;
	uint8_t rt;
	uint8_t rd;
	uint8_t shamt;
	uint8_t funct;
	uint16_t immediate;
	uint32_t address;
} instruction_t;

typedef struct cpu_s
{
	uint32_t pc;
	uint32_t g_registers[32];
} cpu_t;

typedef state_t (*exec_func_t)(cpu_t *cpu, memory_t *memory, instruction_t *inst);

state_t cpu_tick(cpu_t *cpu, memory_t *memory);

instruction_t decode_instruction(uint32_t instruction);

void cpu_init(void);

#endif
