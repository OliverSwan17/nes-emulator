#pragma once
#include "6502.h"

void drawZeroPage();
void drawProgram(u8 pcOffset);
void drawStack(u8 spOffset);
void drawRegisters();
void drawStatusBits();
void drawInstructionToExecute(Instruction instruction);
void drawText(Instruction instruction);

// Unused
void printRegisters(Registers regs);
void printInstruction(Instruction Instruction);
