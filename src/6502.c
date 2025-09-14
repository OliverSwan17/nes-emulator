#include "6502.h"
#include "instructions.h"
#include "draw.h"

extern u8 *rom;
extern size_t romSize;
InstructionMetaData imdLookup[256];
Registers regs;
Memory memory;

void powerUp() {
    regs.A = 0;
    regs.X = 0;
    regs.Y = 0;
    regs.PC = 0xC000; // Will change for NES
    regs.SP = 0xFD;
    regs.SR.byte = 0x24;

    initInstructionMetaData();
    memcpy(memory.program, rom, romSize); // Loading the program into ram

    // Debug.
    FILE *nestestLog = fopen("out/nestestLog.txt", "w");
    Instruction instruction;
    int counter = 0;
    while (1) {
        instruction = identifyInstruction((u8 *)&memory + regs.PC); // Fetch and decode
        if (instruction.opcode.byte == 0x22)
            break;

        draw(instruction);
        writeNestestLog(nestestLog);
        executeInstruction(instruction);

        if (strcmp(instruction.mnemonic, "JSR") != 0 
            && strcmp(instruction.mnemonic, "RTS") != 0 
            && strcmp(instruction.mnemonic, "JMP") != 0
            && strcmp(instruction.mnemonic, "BRK") != 0
            && strcmp(instruction.mnemonic, "RTI") != 0
            ) {regs.PC += instruction.bytes;}
        counter ++;
        if (counter > 6000)
            break;
    }

    draw(instruction);

    fclose(nestestLog);
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

void writeNestestLog(FILE *nestestLog) {
    if (nestestLog != NULL) {
        fprintf(nestestLog, "A:%02X X:%02X Y:%02X P:%02X SP:%02X\n",
                regs.A, 
                regs.X, 
                regs.Y, 
                regs.SR.byte, 
                regs.SP);
        fflush(nestestLog);
    }
}