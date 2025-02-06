#include "draw.h"

extern Registers regs;
extern Memory memory;

void drawText(Instruction instruction) {
    clear();
    drawZeroPage();
    drawStack(regs.SP);
    drawProgram(regs.PC - PROGRAM_OFFSET);
    drawRegisters();
    drawStatusBits();
    drawInstructionToExecute(instruction);
    refresh();
    getch();
}

void drawZeroPage() {
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            printw("%02X ", memory.ram[i * 16 + j]);
        }
        printw("\n");
    }
    printw("\n");

}

void drawStack(u8 spOffset) {
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            if (j + i * 16 == spOffset) {
                attron(A_REVERSE);    // Reverse video (swap foreground and background)
                printw("%02X", memory.ram[0x100 + i * 16 + j]);
                attroff(A_REVERSE);
                printw(" ");
            }
            else
                printw("%02X ", memory.ram[0x100 + i * 16 + j]);
        }
        printw("\n");
    }
    printw("\n");
}

void drawRegisters() {
    mvprintw(0, 50, " A: 0x%02X\n", regs.A);
    mvprintw(1, 50, " X: 0x%02X\n", regs.X);
    mvprintw(2, 50, " Y: 0x%02X\n", regs.Y);
    mvprintw(3, 50, "SP: 0x%02X\n", regs.SP);
    mvprintw(4, 50, "PC: 0x%04X\n", regs.PC);
    mvprintw(5, 50, "SR: 0x%02X\n", regs.SR.byte);
}

void drawStatusBits() {
    mvprintw(0, 62, "C Z I D B R V N\n");
    mvprintw(1, 62, "%u", regs.SR.C);
    mvprintw(1, 64, "%u", regs.SR.Z);
    mvprintw(1, 66, "%u", regs.SR.I);
    mvprintw(1, 68, "%u", regs.SR.D);
    mvprintw(1, 70, "%u", regs.SR.B);
    mvprintw(1, 72, "- ");
    mvprintw(1, 74, "%u", regs.SR.V);
    mvprintw(1, 76, "%u", regs.SR.N);
}

void drawInstructionToExecute(Instruction instruction) {
    char mode[20];
    switch (instruction.addressingMode) {
        case IMPLIED:
            strcpy(mode, "IMPLIED");
            break;
        case ABSOLUTE:
            strcpy(mode, "ABSOLUTE");
            break;
        case IMMEDIATE:
            strcpy(mode, "IMMEDIATE");
            break;
        case ZEROPAGE:
            strcpy(mode, "ZEROPAGE");
            break;
        case ZEROPAGE_X:
            strcpy(mode, "ZEROPAGE_X");
            break;
        case ABSOLUTE_X:
            strcpy(mode, "ABSOLUTE_X");
            break;
        case ABSOLUTE_Y:
            strcpy(mode, "ABSOLUTE_Y");
            break;
        case X_INDIRECT:
            strcpy(mode, "X_INDIRECT");
            break;
        case INDIRECT_Y:
            strcpy(mode, "INDIRECT_Y");
            break;
        default:
            strcpy(mode, "TO_BE_IMPLEMENTED");
            break;
    }

    mvprintw(7, 50, "Mnemonic: %s\n", instruction.mnemonic);
    mvprintw(8, 50, "Addressing Mode: %s", mode);
    mvprintw(9, 50, "Opcode byte: 0x%02X\n", instruction.opcode.byte);
    mvprintw(10, 50, "Operand bytes: 0x%04X\n", instruction.operand.bytes);
    mvprintw(11, 50, "Bytes: %u\n", instruction.bytes);
    mvprintw(12, 50, "Cycles: %u\n", instruction.cycles);
}

void drawProgram(u8 pcOffset) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 16; j++) {
            if (j + i * 16 == pcOffset) {
                attron(A_REVERSE);    // Reverse video (swap foreground and background)
                printw("%02X", memory.program[i * 16 + j]);
                attroff(A_REVERSE);
                printw(" ");
            }
            else
                printw("%02X ", memory.program[i * 16 + j]);
        }
        printw("\n");
    }
}

void printRegisters(Registers regs) {
    printf("------------------------------------------------\n");
    printf("A: 0x%02X\n", regs.A);
    printf("X: 0x%02X\n", regs.X);
    printf("Y: 0x%02X\n", regs.Y);
    printf("SP: 0x%02X\n", regs.SP);
    printf("PC: 0x%04X\n", regs.PC);
    printf("SR: 0x%02X\n\n", regs.SR.byte);
    
    printf("C Z I D B R V N\n");

    printf("%u ", regs.SR.C);
    printf("%u ", regs.SR.Z);
    printf("%u ", regs.SR.I);
    printf("%u ", regs.SR.D);
    printf("%u ", regs.SR.B);
    printf("- ");
    printf("%u ", regs.SR.V);
    printf("%u \n", regs.SR.N);
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
