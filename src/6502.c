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
    u8 code[] = {0x20, 0x06, 0x08, 0xEA, 0xCA, 0x22, 0xEA, 0xEA, 0xEA, 0xE8, 0x60}; // Testing JSR and RTS
    //u8 code[] = {0x48, 0xEA, 0x68, 0x22}; // Testing push and pop A

    //u8 code[] = {0xAA, 0x9A, 0x98, 0x8A, 0xA8}; // Testing transfer instructions
    //            TAX   TXS   TYA   TXA   TAY

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

// Mnemonic, Addressing Mode, Bytes, Cycles
void initInstructionMetaData() {
    imdLookup[0xEA] = (InstructionMetaData){"NOP", IMPLIED, 1, 2};

    imdLookup[0xE8] = (InstructionMetaData){"INX", IMPLIED, 1, 2};
    imdLookup[0xC8] = (InstructionMetaData){"INY", IMPLIED, 1, 2};
    imdLookup[0xCA] = (InstructionMetaData){"DEX", IMPLIED, 1, 2};
    imdLookup[0x88] = (InstructionMetaData){"DEY", IMPLIED, 1, 2};

    imdLookup[0x18] = (InstructionMetaData){"CLC", IMPLIED, 1, 2};
    imdLookup[0xD8] = (InstructionMetaData){"CLD", IMPLIED, 1, 2};
    imdLookup[0x58] = (InstructionMetaData){"CLI", IMPLIED, 1, 2};
    imdLookup[0xB8] = (InstructionMetaData){"CLV", IMPLIED, 1, 2};
    imdLookup[0x38] = (InstructionMetaData){"SEC", IMPLIED, 1, 2};
    imdLookup[0xF8] = (InstructionMetaData){"SED", IMPLIED, 1, 2};
    imdLookup[0x78] = (InstructionMetaData){"SEI", IMPLIED, 1, 2};

    imdLookup[0x20] = (InstructionMetaData){"JSR", ABSOLUTE, 3, 6};
    imdLookup[0x60] = (InstructionMetaData){"RTS", IMPLIED, 1, 6};

    imdLookup[0x48] = (InstructionMetaData){"PHA", IMPLIED, 1, 3};
    imdLookup[0x68] = (InstructionMetaData){"PLA", IMPLIED, 1, 4};

    imdLookup[0xAA] = (InstructionMetaData){"TAX", IMPLIED, 1, 2};
    imdLookup[0xA8] = (InstructionMetaData){"TAY", IMPLIED, 1, 2};
    imdLookup[0xBA] = (InstructionMetaData){"TSX", IMPLIED, 1, 2};
    imdLookup[0x8A] = (InstructionMetaData){"TXA", IMPLIED, 1, 2};
    imdLookup[0x9A] = (InstructionMetaData){"TXS", IMPLIED, 1, 2};
    imdLookup[0x98] = (InstructionMetaData){"TYA", IMPLIED, 1, 2};

    // Custom instructions
    imdLookup[0x22] = (InstructionMetaData){"END", IMPLIED, 1, 0}; // Ends the program
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
