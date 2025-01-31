#include "6502.h"

char* mnemonicLookup[256];
u8 addressingModeLookup[256];
u8 operandCyclesLookup[256];
u8 operandLengthLookup[256];

Registers regs;

void initMnemonicLookup() {
    mnemonicLookup[0xCA] = "DEX";
}

void initAddressingModeLookup() {
    addressingModeLookup[0xCA] = IMPLIED;
}

void initOperandCycles() {
    operandCyclesLookup[0xCA] = 2;
}

void initOperandLength() {
    operandLengthLookup[0xCA] = 0;
}

Instruction identifyInstruction(u8 *binary) {
    Instruction instruction;
    u8 opcode = binary[0];

    instruction.opcode.byte = opcode;
    strcpy(instruction.mnemonic, mnemonicLookup[opcode]);
    instruction.adressingMode = addressingModeLookup[opcode];
    instruction.cycles = operandCyclesLookup[opcode];
    instruction.operandLength = operandLengthLookup[opcode];

    if (instruction.operandLength) {
        instruction.operand.lowByte = binary[1];
        if (instruction.operandLength == 2)
            instruction.operand.highByte = binary[2];
    }

    return instruction;
}

void DEX() {
    regs.X -= 1;
}

void executeInstruction(Instruction instruction) {
    if (strcmp(instruction.mnemonic, "DEX") == 0)
        DEX();
}
