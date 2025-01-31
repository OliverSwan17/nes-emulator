#include "6502.h"

InstructionMetaData imdLookup[256];

Registers regs;

void initInstructionMetaData() {
    imdLookup[0xCA] = (InstructionMetaData){"DEX", IMPLIED, 1, 2};
    imdLookup[0xEA] = (InstructionMetaData){"NOP", IMPLIED, 1, 2};
    imdLookup[0x20] = (InstructionMetaData){"JSR", ABSOLUTE, 3, 6};
}

Instruction identifyInstruction(u8 *binary) {
    Instruction instruction = {0};
    InstructionMetaData imd = imdLookup[binary[0]];
    
    instruction.opcode.byte = binary[0];
    strcpy((char *) instruction.mnemonic, imd.mnemonic);
    instruction.addressingMode = imd.adressingMode;
    instruction.cycles = imd.cycles;
    instruction.bytes = imd.bytes;

    if (instruction.bytes > 1) {
        instruction.operand.lowByte = binary[1];
        if (instruction.bytes > 2)
            instruction.operand.highByte = binary[2];
    }

    return instruction;
}

void DEX() {
    regs.X -= 1;
}

void NOP() {
    return;
}

void JSR() {
    // Push PC + 2 to the stack
    // PC = Memory address in  
}

void executeInstruction(Instruction instruction) {
    if (strcmp(instruction.mnemonic, "DEX") == 0)
        DEX();
    if (strcmp(instruction.mnemonic, "NOP") == 0)
        NOP();
    if (strcmp(instruction.mnemonic, "JSR") == 0)
        JSR();
}
