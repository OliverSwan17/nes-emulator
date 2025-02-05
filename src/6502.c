#include "6502.h"
#include "instructions.h"
#include "draw.h"

InstructionMetaData imdLookup[256];
Registers regs;
Memory memory;

void powerUp() {
    regs.A = 5;
    regs.X = 0;
    regs.Y = 10;
    regs.PC = sizeof(Memory) - sizeof(memory.program); // Will change for NES
    regs.SP = 0xFF;
    regs.SR.C = 0;
    regs.SR.Z = 0;
    regs.SR.I = 1;
    regs.SR.D = 0;
    regs.SR.B = 0;
    regs.SR.R = 0;
    regs.SR.V = 0;
    regs.SR.N = 0;

    initInstructionMetaData();
    //u8 code[] = {0x20, 0x06, 0x08, 0xEA, 0xCA, 0x22, 0xEA, 0xEA, 0xEA, 0xE8, 0x60}; // Testing JSR and RTS
    //u8 code[] = {0xA9, 0x69, 0x48, 0xA9, 0x21, 0xEA, 0x68, 0x22}; // Testing push and pop A with LDA

    //u8 code[] = {0xAA, 0x9A, 0x98, 0x8A, 0xA8}; // Testing transfer instructions
    //            TAX   TXS   TYA   TXA   TAY

    u8 code[] = {0xA9, 0x17, 0x85, 0x21, 0x48, 0xA9, 0x00, 0xA5, 0x21, 0xA9, 0x00, 0x68, 0x22};

    memcpy(memory.program, code, sizeof(code));

    Instruction instruction;
    while (1) {
        instruction = identifyInstruction((u8 *)&memory + regs.PC); // Fetch and decode
        if (instruction.opcode.byte == 0x22)
            break;

        drawText(instruction);
        executeInstruction(instruction); // Execute

        if (strcmp(instruction.mnemonic, "JSR") != 0 && strcmp(instruction.mnemonic, "RTS") != 0) // Add other branch and jumps in the future
            regs.PC += instruction.bytes;
    }

    drawText(instruction);
}

Instruction identifyInstruction(u8 *binary) {
    Instruction instruction = {0};
    InstructionMetaData imd = imdLookup[binary[0]];
    
    instruction.opcode.byte = binary[0];
    strcpy((char *) instruction.mnemonic, imd.mnemonic);
    instruction.addressingMode = imd.adressingMode;
    instruction.cycles = imd.cycles;
    instruction.bytes = imd.bytes;

    // Handles instructions of different lengths
    if (instruction.bytes > 1) {
        instruction.operand.lowByte = binary[1];
        if (instruction.bytes > 2)
            instruction.operand.highByte = binary[2];
    }

    return instruction;
}
