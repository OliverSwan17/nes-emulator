#include "utils.h"

void displayRegisters(Registers regs) {
    printf("------------------------------------------------\n");
    printf("A: %u\n", regs.A);
    printf("X: %u\n", regs.X);
    printf("Y: %u\n", regs.Y);
    printf("SP: %u\n", regs.SP);
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