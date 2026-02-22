# Metal-MIPS: Bare-Metal Architecture & Execution Engine

Metal-MIPS is a custom-built, modular Instruction Set Simulator (ISS) for a subset of the MIPS32 architecture. Written in strictly compliant C (gnu89 standard), this engine simulates the physical realities of a CPU and its memory without relying on high-level abstractions or operating system utilities.

This document serves as the comprehensive architectural reference and engineering manual for the project.

---

## 1. System Architecture & Components

The simulator is rigidly divided into four decoupled subsystems, adhering to the principle of Separation of Concerns:

### A. The Engine (`engine/`)
The orchestrator of the simulation. It acts as the physical motherboard linking the CPU and RAM.
* **Memory Allocation:** Safely allocates the requested RAM size in the host's user space using `calloc` to ensure zero-initialized physical memory state, preventing non-deterministic behavior.
* **Execution Loop:** Continuously pumps the clock cycle by calling `cpu_tick` until a HALT signal is trapped or a fatal error (e.g., Segfault) occurs.

### B. The Memory Model (`memory/`)
* **Structure:** A 1D flat byte array (`uint8_t *mem`) and a size tracker (`uint32_t size`).
* **Bounds Checking:** Every memory access (Fetch, Load, Store) undergoes strict bounds checking. If `address + 3 >= memory.size`, the system traps a `SIM_ERR_MEM_OUT_OF_BOUNDS` exception.
* **Endianness:** Implements Big-Endian byte ordering manually via bitwise shifting during instruction fetch and memory I/O operations.

### C. The CPU Core (`cpu/`)
Maintains the internal state of the processor:
* `uint32_t pc`: The Program Counter.
* `uint32_t g_registers[32]`: General-purpose registers. `$0` is hardwired to zero (writes are discarded).
* `uint32_t hi, lo`: Reserved registers for future implementation of 64-bit multiplication and division results.

---

## 2. The Execution Pipeline

The core performs a standard 3-stage pipeline sequentially per tick:

1.  **Fetch:** Reads a 32-bit word from the simulated RAM at the address specified by the `pc`.
    ```c
    uint32_t inst = (mem[pc] << 24) | (mem[pc+1] << 16) | (mem[pc+2] << 8) | mem[pc+3];
    ```
    The `pc` is immediately incremented by 4.
2.  **Decode:**
    Extracts instruction fields (`opcode`, `rs`, `rt`, `rd`, `shamt`, `funct`, `immediate`, `address`) using bitwise masks (e.g., `(inst >> 26) & 0x3F`).
3.  **Execute (The O(1) Dispatcher):**
    Instead of monolithic `switch-case` statements, execution is routed via a Two-Level Function Pointer Dispatch Table.

### The O(1) Dispatch Table
The `opcode_table` holds pointers to execution functions. 
* If `opcode == 0x00` (R-Type), the table routes to a secondary dispatcher (`route_r_type`), which indexes into the `r_type_table` using the `funct` field.
* This design ensures O(1) time complexity for instruction routing and makes the ISA infinitely extensible.

---

## 3. Instruction Set Architecture (ISA)

The current core supports a Turing-complete subset of instructions capable of executing loops, conditionals, and memory I/O.

| Type | Name | Opcode | Funct | Operation |
| :--- | :--- | :---: | :---: | :--- |
| R | `add` | 0x00 | 0x20 | `$rd = $rs + $rt` |
| R | `sub` | 0x00 | 0x22 | `$rd = $rs - $rt` |
| R | `slt` | 0x00 | 0x2A | `$rd = ($rs < $rt) ? 1 : 0` |
| I | `addi` | 0x08 | N/A | `$rt = $rs + SignExt(imm)` |
| I | `lw` | 0x23 | N/A | `$rt = Memory[$rs + SignExt(imm)]` |
| I | `sw` | 0x2B | N/A | `Memory[$rs + SignExt(imm)] = $rt` |
| I | `beq` | 0x04 | N/A | `if ($rs == $rt) pc += (SignExt(imm) << 2)` |
| J | `j` | 0x02 | N/A | `pc = (pc & 0xF0000000) \| (address << 2)` |

### The HALT Condition (Graceful Exit)
Real CPUs do not "stop" reading memory; they execute until power loss. To allow our simulator to terminate gracefully and dump the final state, a custom **HALT instruction** is implemented.
* **Custom Opcode:** `0x3F` (Binary: `111111`)
* **Binary Representation:** `0xFC000000`
When the CPU decodes `0x3F`, it returns `SIM_HALT`. The Engine catches this signal, breaks the execution loop, triggers the register dump, and safely frees the allocated memory.

---

## 4. Usage & Examples

### Compilation
The project relies on a `Makefile`. It builds using `gcc` with strict flags (`-Wall -Werror -Wextra -pedantic -std=gnu89`).
```bash
make
---

## 📂 Directory Structure

```text
.
├── Makefile              # Build automation
├── README.md             # Project documentation
├── main.c                # Application entry point
├── engine/               # Simulation lifecycle management
│   ├── engine.c
│   └── engine.h
├── memory/               # Virtual RAM definitions
│   └── memory.h
├── io/                   # Binary loading and state dumping
│   ├── display.c
│   ├── loader.c
│   └── io.h
└── cpu/                  # Processor core and ISA
    ├── cpu.c
    ├── cpu.h
    └── isa/
        ├── isa.h         # Execution function prototypes
        ├── r-type/       # Register-to-Register operations (add, sub, slt)
        ├── i-type/       # Immediate and Memory operations (lw, sw, beq, addi)
        └── j-type/       # Jump operations (j)
