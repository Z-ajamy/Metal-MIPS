# Metal-MIPS: Bare-Metal CPU Core Architecture & Simulator

Metal-MIPS is a modular, high-performance CPU instruction set simulator written in strict ANSI C (gnu89 standard). It implements a Turing-complete subset of a 32-bit MIPS-like architecture.

This document serves as the architectural blueprint and technical reference for the simulator. It outlines the core engineering decisions, data path designs, and memory models used to bridge the gap between abstract computer architecture and low-level systems programming.

---

## 🧠 Core Architectural Decisions & Interview Reference

When designing a systems-level emulator, performance, memory safety, and architectural decoupling are paramount. Below are the specific design choices implemented in this project.

### 1. O(1) Instruction Routing (The Dispatch Tables)
**The Problem:** Traditional beginner emulators use massive `switch/case` statements to decode and execute opcodes. In a real CPU, decoding happens in parallel hardware. In software, a giant `switch` statement causes massive branch prediction penalties and cache misses.

**The Solution:** Metal-MIPS utilizes **Two-Level Function Pointer Dispatch Tables**.
* We define an array of function pointers: `static exec_func_t opcode_table[64];`
* The CPU uses the extracted 6-bit `opcode` as a direct array index.
* If the `opcode` is `0x00` (R-Type), the simulator jumps to a secondary table `r_type_table` and uses the 6-bit `funct` field as the index.



**Technical Trade-off:** We sacrifice a tiny amount of memory (pointers array overhead) to gain O(1) execution routing, eliminating branch condition overhead and mimicking hardware-level multiplexer behavior.

### 2. Memory Model & Endianness Management
**The Problem:** Host machines (like x86 Intel processors) are typically *Little-Endian*, while network protocols and architectures like MIPS are strictly *Big-Endian*. If we read a 32-bit integer directly from the host's memory, the bytes will be reversed.

**The Solution:** The `memory_t` struct treats RAM exclusively as a flat array of raw bytes (`uint8_t *mem`). 
To ensure architectural purity regardless of the host machine, the Fetch cycle manually constructs the 32-bit instruction using explicit bitwise shifts:
`uint32_t inst = (mem[pc] << 24) | (mem[pc+1] << 16) | (mem[pc+2] << 8) | mem[pc+3];`



* **Memory Safety:** The simulator enforces rigorous bounds-checking (`pc + 3 >= memory->size`) *before* every fetch or memory access (`lw`, `sw`). Memory is allocated via `calloc` rather than `malloc` to ensure the initial state is zeroed out, preventing the execution of garbage data.

---

## ⚙️ The Execution Pipeline (State Machine)

The simulator operates as a continuous state machine driven by the `engine_run` loop.



### I. The Fetch Stage
The CPU accesses the memory via the Program Counter (`PC`). Once the 4 bytes are fetched and packed into a `uint32_t`, the `PC` is immediately incremented by 4 (`pc += 4`). This is a critical hardware realism: when the instruction executes (e.g., a branch or jump), the `PC` is *already* pointing to the next sequential instruction.

### II. The Decode Stage (Bitwise Masking)
Instead of dealing with strings or high-level types, the `decode_instruction` function extracts architectural fields using logical bitwise `AND` masks and Right Shifts.
For example, extracting the opcode (top 6 bits): `(inst >> 26) & 0x3F;`

### III. The Execute & Writeback Stage
The operation is passed to isolated execution functions (e.g., `exec_add`). These functions modify the `g_registers` array.
**Safety Rule:** Register `$0` (`g_registers[0]`) is hardwired to zero. Every execution function must verify `if (inst->rd != 0)` before writing back to prevent destroying the zero register integrity.

---

## 🛑 The Stopping Condition (Graceful Exit)

Real CPUs are blind; they do not know what a "program" is. They simply read memory continuously until power is lost. If an emulator does not have a stopping condition, the `PC` will eventually march into uninitialized memory, interpret garbage bytes as instructions, and trigger a Segmentation Fault.

**The Implementation:**
To achieve a "Graceful Exit", Metal-MIPS defines a custom **HALT Pseudo-instruction**.
* **Custom Opcode:** We reserved the opcode `0x3F` (Binary: `111111`, Hex: `0xFC000000`).
* **Behavior:** When the dispatch table routes `0x3F` to `exec_halt()`, the function returns a specific enum state: `SIM_HALT` instead of `SIM_SUCCESS`.
* **Engine Handling:** The Engine detects this state, safely breaks the infinite `while (eng->is_running)` loop, avoids throwing an error, dumps the final CPU register state, and frees the allocated RAM.

---

## 🚀 Usage & Integration Example

The simulator is a pure command-line tool. It takes two arguments: the raw binary file containing the machine code, and the amount of simulated RAM to allocate.

### 1. Providing Machine Code
Because we operate at bare metal, the simulator does not parse assembly text (like `.s` files). It expects pure binary byte streams. You can generate a test binary using a hex editor or a simple Python script:

```bash
# Example: Generate a binary that executes an infinite loop or arithmetic operations
python3 -c "import sys; sys.stdout.buffer.write(bytes([0x20, 0x01, 0x00, 0x0A, 0xFC, 0x00, 0x00, 0x00]))" > program.bin

---

## 🛠️ Supported Instruction Set (ISA)

The current ISA subset includes the 7 most critical instructions required to run
complex algorithms (like loops and conditional branches), plus a custom `HALT` instruction for graceful exits.

| Type | Instruction | Opcode | Funct | Description |
| :--- | :--- | :---: | :---: | :--- |
| **R-Type** | `add` | 0x00 | 0x20 | Adds registers `rs` and `rt`, stores in `rd`. |
| **R-Type** | `sub` | 0x00 | 0x22 | Subtracts `rt` from `rs`, stores in `rd`. |
| **R-Type** | `slt` | 0x00 | 0x2A | Set on Less Than (used for conditionals). |
| **I-Type** | `addi` | 0x08 | N/A | Add immediate (sign-extended) to `rs`, store in `rt`. |
| **I-Type** | `lw` | 0x23 | N/A | Load 32-bit word from memory into `rt`. |
| **I-Type** | `sw` | 0x2B | N/A | Store 32-bit word from `rt` into memory. |
| **I-Type** | `beq` | 0x04 | N/A | Branch to PC-relative address if `rs == rt`. |
| **J-Type** | `j` | 0x02 | N/A | Unconditional absolute jump. |
| **Custom**| `HALT` | 0x3F | N/A | Safely terminates the simulation engine. |

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
