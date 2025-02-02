#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef uint8_t byte;
typedef uint8_t u8;
typedef uint16_t u16;

typedef struct Registers {
    u8 A;  // Accumulator
    u8 X;  // Index
    u8 Y;  // Index
    u16 PC; // Program counter
    u8 SP; // Stack pointer
    
    // Status Register (SR)
    union {
        u8 byte;
        struct {
            u8 C : 1; // Carry
            u8 Z : 1; // Zero
            u8 I : 1; // Interrupt
            u8 D : 1; // Decimal
            u8 B : 1; // Break
            u8 R : 1; // Ignored
            u8 V : 1; // Overflow
            u8 N : 1; // Negative
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

typedef struct Instruction {
    union {
        u8 byte;
        struct {
            u8 lowNibble  : 4;
            u8 highNibble : 4;
        };
    } opcode;

    union {
        u16 bytes;
        struct {
            u8 lowByte;
            u8 highByte;
        };
        
    } operand;
    
    // Meta data
    char mnemonic[4];
    AddressingMode addressingMode;
    u8 cycles;
    u8 bytes;
} Instruction;

typedef struct InstructionMetaData {
    char mnemonic[4];
    AddressingMode adressingMode;
    u8 bytes;
    u8 cycles;
} InstructionMetaData;

typedef struct Memory {
    u8 ram[2048];
    u8 program[256];
} Memory;

// Functions
void initInstructionMetaData();
Instruction identifyInstruction(u8 *binary);
void executeInstruction(Instruction instruction);
void powerUp();

// Opcodes
void NOP();

void INX();
void INY();
void DEX();
void DEY();


void SEC();
void SED();
void SEI();
void CLC();
void CLD();
void CLI();
void CLV();

void JSR(Instruction instruction);
void RTS();

void PHA();
void PLA();

void TAX();
void TAY();
void TSX();
void TXA();
void TXS();
void TYA();

// Utils
void displayRegisters(Registers regs);
void printInstruction(Instruction Instruction);

// Macros
#define UPDATE_Z_FLAG(value) \
    do { \
        regs.SR.Z = ((value) == 0) ? 1 : 0; \
    } while (0)

#define UPDATE_N_FLAG(value) \
    do { \
        regs.SR.N = ((value) >> 7) ? 1 : 0; \
    } while (0)
