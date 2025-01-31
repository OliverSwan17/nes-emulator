# pragma once

#include "6502.h"

void displayRegisters(Registers regs);
void initMnemonicLookup();
void initAddressingModeLookup();
void initOperandCycles();
void initOperandLength();
Instruction identifyInstruction(u8 *binary);
void DEX();
void executeInstruction(Instruction instruction);
