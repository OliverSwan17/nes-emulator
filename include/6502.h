#pragma once
#include "defs.h"

// Based on https://www.nesdev.org/wiki/CPU_registers and https://www.nesdev.org/wiki/Status_flags
typedef struct Registers {
    u8 A;  // Accumulator
    u8 X;  // Index
    u8 Y;  // Index
    u16 PC; // Program counter
    u8 SP; // Stack pointer
    
    // Status Register (SR)
    union {
        u8 SR; // The whole Status Register byte
        struct {
            u8 Carry : 1;            // 0
            u8 Zero : 1;             // 1
            u8 InterruptDisable : 1; // 2
            u8 Decimal : 1;          // 3
            u8 Break : 1;            // 4
            u8 Ignored : 1;          // 5
            u8 Overflow : 1;         // 6
            u8 Negative : 1;         // 7
        };
    } SR;
} Registers;

typedef enum {
    ACCUMULATOR,
    ABSOLUTE,
    ABSOLUTE_X,
    ABSOLUTE_Y,
    IMMEDIATE,
    IMPLIED,
    INDIRECT,
    X_INDIRECT,
    INDIRECT_Y,
    RELATIVE,
    ZEROPAGE,
    ZEROPAGE_X,
    ZEROPAGE_Y,
} AddressingMode;

typedef enum {
    ADC, AND, ASL, BCC,	BCS, BEQ, BIT, BMI,	BNE, BPL, BRK, BVC,	BVS, CLC,
    CLD, CLI, CLV, CMP,	CPX, CPY, DEC, DEX,	DEY, EOR, INC, INX,	INY, JMP,
    JSR, LDA, LDX, LDY,	LSR, NOP, ORA, PHA,	PHP, PLA, PLP, ROL,	ROR, RTI,
    RTS, SBC, SEC, SED,	SEI, STA, STX, STY,	TAX, TAY, TSX, TXA,	TXS, TYA,
} Mnemonic;

typedef struct Instruction {
    u8 opcode;
    u16 operand;

    enum mnemonic;
    AddressingMode adressingMode;
    u8 cycles;
} Instruction;
