#pragma once
#include "6502.h"
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
