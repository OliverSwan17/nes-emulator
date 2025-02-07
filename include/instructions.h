#pragma once
#include "6502.h"

#define READ_LLHH_RAM(llAdr) ((memory.ram[llAdr + 1] << 8) | memory.ram[llAdr])
#define READ_RAM(addr) (memory.ram[addr])
#define WRITE_RAM(addr, val) (memory.ram[addr] = val)

#define ZEROPAGE_X_ADDR(lowByte, X) ((lowByte) + (X))
#define ABSOLUTE_ADDR(lowByte, highByte) ((highByte << 8) | lowByte)
#define ABSOLUTE_X_ADDR(operand, X) (operand + X)
#define ABSOLUTE_Y_ADDR(operand, Y) (operand + Y)

#define X_INDIRECT_ADDR(lowByte, X) \
    ((memory.ram[lowByte + X + 1] << 8) | (memory.ram[(lowByte) + (X)]))

#define INDIRECT_Y_ADDR(lowByte, Y) \
    (((memory.ram[lowByte + 1] << 8) | memory.ram[lowByte]) + Y)



#define UPDATE_Z_FLAG(value) (regs.SR.Z = ((value) == 0))
#define UPDATE_N_FLAG(value) (regs.SR.N = ((value) >> 7))
#define UPDATE_C_FLAG(value) (regs.SR.C) = ((value))




void executeInstruction(Instruction instruction);

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

void LDA(Instruction instruction);
void STA(Instruction instruction);

void CPX(Instruction instruction);
void CPY(Instruction instruction);