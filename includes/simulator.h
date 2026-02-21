#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stdint.h>
#include <inttypes.h>

/**
 * struct cpu_s - Represents the core state of the simulated CPU
 * @pc: The Program Counter, holding the memory address of the next instruction
 * @g_registers: Array of 32 general-purpose 32-bit registers.
 * Note: Register 0 ($zero) must always remain 0.
 *
 * Description: This structure encapsulates the entire hardware state of the
 * CPU at any given cycle, excluding the external memory.
 */
typedef struct cpu_s
{
	uint32_t pc;
	uint32_t g_registers[32];
} cpu_t;

/**
 * struct memory_s - Represents the simulated flat-model physical memory
 * @mem: Pointer to the dynamically allocated byte array representing RAM
 * @size: The total size of the allocated memory in bytes
 *
 * Description: Acts as the main memory (RAM) for the simulator.
 * All memory accesses must be bounds-checked against @size.
 */
typedef struct memory_s
{
	uint8_t *mem;
	uint32_t size;
} memory_t;

/**
 * struct instruction_s - Represents a decoded 32-bit instruction
 * @op: Operation code (opcode) - 6 bits
 * @rs: First source register operand - 5 bits
 * @rt: Second source register operand - 5 bits
 * @rd: Destination register operand - 5 bits
 * @shamt: Shift amount - 5 bits
 * @funct: Function code (extends opcode) - 6 bits
 * @immediate: 16-bit immediate value or offset. Can represent negative values.
 * @address: 26-bit target address for jump instructions
 *
 * Description: A unified structure holding all possible fields extracted
 * from a raw 32-bit machine instruction. Unused fields for a specific
 * instruction type are simply ignored during execution.
 */
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

/**
 * enum state_e - Defines the execution status codes of the simulator
 * @SIM_SUCCESS: Instruction executed successfully
 * @SIM_ERR_INVALID_OPCODE: Unrecognized or unsupported instruction opcode
 * @SIM_ERR_MEM_OUT_OF_BOUNDS: Attempted to access memory beyond allocated size
 *
 * Description: Used as return values for execution functions to indicate
 * the health of the simulation cycle.
 */
typedef enum state_e
{
	SIM_SUCCESS = 0,
	SIM_ERR_INVALID_OPCODE,
	SIM_ERR_MEM_OUT_OF_BOUNDS
} state_t;

/**
 * decode_instruction - Parses a raw 32-bit machine code into structural fields
 * @instruction: The raw 32-bit instruction fetched from memory
 *
 * Return: An instruction_t structure populated with the extracted bit fields
 */
instruction_t decode_instruction(uint32_t instruction);

/**
 * r_format - Executes an R-type instruction (Register-to-Register)
 * @cpu: Pointer to the current CPU state
 * @memory: Pointer to the simulated memory
 * @instruction: Pointer to the decoded instruction structure
 *
 * Return: state_t indicating success or failure
 */
state_t r_format(cpu_t *cpu, memory_t *memory, instruction_t *instruction);

/**
 * i_format - Executes an I-type instruction (Immediate/Data Transfer/Branch)
 * @cpu: Pointer to the current CPU state
 * @memory: Pointer to the simulated memory
 * @instruction: Pointer to the decoded instruction structure
 *
 * Return: state_t indicating success or failure
 */
state_t i_format(cpu_t *cpu, memory_t *memory, instruction_t *instruction);

/**
 * j_format - Executes a J-type instruction (Unconditional Jump)
 * @cpu: Pointer to the current CPU state
 * @memory: Pointer to the simulated memory
 * @instruction: Pointer to the decoded instruction structure
 *
 * Return: state_t indicating success or failure
 */
state_t j_format(cpu_t *cpu, memory_t *memory, instruction_t *instruction);

#endif /* SIMULATOR_H */
