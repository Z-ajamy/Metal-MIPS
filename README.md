# Metal-MIPS: A Bare-Metal CPU Core Simulator

Metal-MIPS is a custom, modular, and highly efficient CPU instruction set simulator written entirely in C (gnu89 standard). It is designed to demystify the "black box" of modern processors by implementing a minimal viable product (MVP) of a MIPS-like architecture. 

This project is a direct application of the concepts presented in Chapter 2 of the classic textbook *"Computer Organization and Design: The Hardware/Software Interface"*.

## 🎯 Project Goal & Philosophy
The primary goal of Metal-MIPS is **educational mastery through construction ("Build-it-yourself")**. 

Instead of relying on high-level abstractions or existing emulators, this project implements the physical realities of hardware—such as bitwise decoding, flat memory modeling, sign-extension, and CPU lifecycles—entirely in software. It aims to bridge the gap between abstract computer architecture theory and low-level system programming, serving as a foundational step toward understanding operating systems, compilers, and infrastructure engineering.

---

## 🏗️ System Architecture

The simulator is built using a strict **Layered Architecture** to enforce separation of concerns. The CPU does not know about files, and the Engine does not care about how instructions are executed.



### 1. The Engine (`engine/`)
Acts as the simulated motherboard. It allocates the physical RAM, resets the CPU registers, loads the program via the IO layer, and pumps the continuous clock cycles (`cpu_tick`).

### 2. The Memory (`memory/`)
A flat, contiguous array of bytes (`uint8_t *`) dynamically allocated in user space. It enforces strict bounds-checking to prevent segmentation faults during simulation, acting as the system's RAM.

### 3. The CPU Core (`cpu/`)
The heart of the simulator. It maintains the physical state:
* **Program Counter (PC):** A 32-bit pointer to the current instruction.
* **Register File:** An array of 32 general-purpose 32-bit registers (where `$0` is hardwired to zero).



[Image of CPU Fetch Decode Execute cycle]


The CPU operates on a strict **Fetch -> Decode -> Execute** loop:
1. **Fetch:** Reads 4 bytes from RAM in Big-Endian format.
2. **Decode:** Uses bitwise masks and shifts to parse the 32-bit instruction into logical fields (`opcode`, `rs`, `rt`, `rd`, `shamt`, `funct`, `immediate`, `address`).
3. **Execute:** Routes the decoded instruction to the correct arithmetic or logic unit.

---

## ⚡ The Secret Sauce: O(1) Execution Pipeline

To achieve maximum execution speed and avoid massive, slow `switch/case` statements, Metal-MIPS uses a **Two-Level Function Pointer Dispatch Table**.



Instructions are mapped directly to their execution functions via an array of function pointers. 
* The primary table routes instructions based on their 6-bit `opcode`.
* For R-type instructions (where `opcode == 0`), a secondary table routes execution based on the 6-bit `funct` field.
This guarantees O(1) time complexity for instruction routing, mirroring the speed of hardware decoders.

---

## 🛠️ Supported Instruction Set (ISA)

The current ISA subset includes the 7 most critical instructions required to run complex algorithms (like loops and conditional branches), plus a custom `HALT` instruction for graceful exits.

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
