#pragma once
#include "6502.h"

#define READ_LLHH_RAM(llAdr)  ((memory.ram[((llAdr) + 1) & 0xFF] << 8) | memory.ram[(llAdr)])
#define READ_RAM(addr) (memory.ram[(addr)])
#define WRITE_RAM(addr, val) (memory.ram[(addr)] = (val))

#define ZEROPAGE_X_ADDR(lowByte, X) (((lowByte) + (X)) & 0xFF)
#define ZEROPAGE_Y_ADDR(lowByte, Y) ((lowByte) + (Y))
#define ABSOLUTE_ADDR(lowByte, highByte) ((highByte << 8) | lowByte)
#define ABSOLUTE_X_ADDR(operand, X) ((operand) + (X))
#define ABSOLUTE_Y_ADDR(operand, Y) ((operand) + (Y))

#define X_INDIRECT_ADDR(lowByte, X) \
    ((memory.ram[((lowByte) + (X) + 1) & 0xFF] << 8) | (memory.ram[((lowByte) + (X)) & 0xFF]))

#define INDIRECT_Y_ADDR(lowByte, Y) \
    ((((memory.ram[((lowByte) + 1) & 0xFF] << 8) | memory.ram[(lowByte)]) + (Y)) & 0xFFFF)



#define UPDATE_Z_FLAG(value) (regs.SR.Z = ((value) == 0))
#define UPDATE_N_FLAG(value) (regs.SR.N = ((value) >> 7))
#define UPDATE_C_FLAG(value) (regs.SR.C) = ((value))




void executeInstruction(Instruction);
void Bitwise(Instruction);

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
void JSR(Instruction);
void RTS();
void PHA();
void PLA();
void TAX();
void TAY();
void TSX();
void TXA();
void TXS();
void TYA();
void LDA(Instruction);
void STA(Instruction);
void LDX(Instruction);
void LDY(Instruction);
void STX(Instruction);
void STY(Instruction);
void CPX(Instruction);
void CPY(Instruction);
void INC(Instruction);
void DEC(Instruction);
void AND(Instruction);
void ORA(Instruction);
void EOR(Instruction);
void ASL(Instruction);
void LSR(Instruction);
void ROL(Instruction);
void ROR(Instruction);
void JMP(Instruction);
void CMP(Instruction);
void BIT(Instruction);
void PHP();
void PLP();
void BCC(Instruction);
void BCS(Instruction);
void BEQ(Instruction);
void BMI(Instruction);
void BNE(Instruction);
void BPL(Instruction);
void BVC(Instruction);
void BVS(Instruction);
void BRK();
void RTI();
void ADC(Instruction);
void SBC(Instruction);
