#include "6502.h"

InstructionMetaData imdLookup[256];

Registers regs;
Memory memory;

void powerUp() {
    regs.A = 0;
    regs.X = 0;
    regs.Y = 0;
    regs.PC = sizeof(Memory) - sizeof(memory.program); // Will change for NES
    regs.SP = 0xFD;
    regs.SR.C = 0;
    regs.SR.Z = 0;
    regs.SR.I = 1;
    regs.SR.D = 0;
    regs.SR.B = 0;
    regs.SR.R = 0;
    regs.SR.V = 0;
    regs.SR.N = 0;

    displayRegisters(regs);

    u8 code[] = {0xCA, 0xEA, 0xEA, 0x20, 0xCB, 0xAB};
    memcpy(memory.program, code, sizeof(code));

    int remainingInstructions = 4;
    Instruction instruction;
    while (remainingInstructions) {
        instruction = identifyInstruction((u8 *)&memory + regs.PC);
        printInstruction(instruction);
        executeInstruction(instruction);
        regs.PC += instruction.bytes;
        displayRegisters(regs);
        remainingInstructions--;
    }
}

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

void displayRegisters(Registers regs) {
    printf("------------------------------------------------\n");
    printf("A: %u\n", regs.A);
    printf("X: %u\n", regs.X);
    printf("Y: %u\n", regs.Y);
    printf("SP: %u\n", regs.SP);
    printf("PC: %u\n", regs.PC);
    printf("SR: %u\n\n", regs.SR.byte);
    
    printf("C: %u\n", regs.SR.C);
    printf("Z: %u\n", regs.SR.Z);
    printf("I: %u\n", regs.SR.I);
    printf("D: %u\n", regs.SR.D);
    printf("B: %u\n", regs.SR.B);
    printf("R: -\n");
    printf("V: %u\n", regs.SR.V);
    printf("N: %u\n", regs.SR.N);
}

void printInstruction(Instruction instruction) {
    printf("------------------------------------------------\n");
    printf("Opcode byte: 0x%02X\n", instruction.opcode.byte);
    printf("Low Nibble: 0x%X\n", instruction.opcode.lowNibble);
    printf("High Nibble: 0x%X\n\n", instruction.opcode.highNibble);

    printf("Operand bytes: 0x%04X\n", instruction.operand.bytes);
    printf("Low Byte: 0x%02X\n", instruction.operand.lowByte);
    printf("High Byte: 0x%02X\n\n", instruction.operand.highByte);

    printf("Mnemonic: %s\n", instruction.mnemonic);

    printf("Addressing Mode: ");
    switch (instruction.addressingMode) {
        case IMPLIED:
            printf("IMPLIED\n");
            break;
        case ABSOLUTE:
            printf("ABSOLUTE\n");
            break;
        default:
            printf("TO_BE_IMPLEMENTED\n");
            break;
    }

    printf("Cycles: %u\n", instruction.cycles);
    printf("Bytes: %u\n", instruction.bytes);
}
