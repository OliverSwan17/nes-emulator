#include "6502.h"

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

    displayRegisters(regs);

    //u8 code[] = {0x20, 0x05, 0x08, 0xEA, 0xCA, 0xEA, 0xEA, 0xEA, 0xE8, 0x60}; // Testing JSR and RTS
    // u8 code[] = {0x48, 0xEA, 0x68}; // Testing push and pop A
    
    u8 code[] = {0xAA, 0x9A, 0x98, 0x8A, 0xA8};
    //            TAX   TXS   TYA   TXA   TAY

    memcpy(memory.program, code, sizeof(code));

    int remainingInstructions = 5;
    Instruction instruction;
    while (remainingInstructions) {
        instruction = identifyInstruction((u8 *)&memory + regs.PC);
        printInstruction(instruction);
        executeInstruction(instruction);

        if (strcmp(instruction.mnemonic, "JSR") != 0 && strcmp(instruction.mnemonic, "RTS") != 0) // Add other branch and jumps in the future
            regs.PC += instruction.bytes;

        displayRegisters(regs);
        remainingInstructions--;
    }
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

void executeInstruction(Instruction instruction) {
    if (strcmp(instruction.mnemonic, "NOP") == 0)
        NOP();

    if (strcmp(instruction.mnemonic, "INX") == 0)
        INX();
    if (strcmp(instruction.mnemonic, "INY") == 0)
        INY();
    if (strcmp(instruction.mnemonic, "DEX") == 0)
        DEX();
    if (strcmp(instruction.mnemonic, "DEY") == 0)
        DEY();

    if (strcmp(instruction.mnemonic, "CLC") == 0)
        CLC();
    if (strcmp(instruction.mnemonic, "CLD") == 0)
        CLD();
    if (strcmp(instruction.mnemonic, "CLI") == 0)
        CLI();
    if (strcmp(instruction.mnemonic, "CLV") == 0)
        CLV();
    if (strcmp(instruction.mnemonic, "SEC") == 0)
        SEC();
    if (strcmp(instruction.mnemonic, "SED") == 0)
        SED();
    if (strcmp(instruction.mnemonic, "SEI") == 0)
        SEI();

    if (strcmp(instruction.mnemonic, "JSR") == 0)
        JSR(instruction);
    if (strcmp(instruction.mnemonic, "RTS") == 0)
        RTS();

    if (strcmp(instruction.mnemonic, "PHA") == 0)
        PHA();
    if (strcmp(instruction.mnemonic, "PLA") == 0)
        PLA();

    if (strcmp(instruction.mnemonic, "TAX") == 0)
        TAX();
    if (strcmp(instruction.mnemonic, "TAY") == 0)
        TAY();
    if (strcmp(instruction.mnemonic, "TSX") == 0)
        TSX();
    if (strcmp(instruction.mnemonic, "TXA") == 0)
        TXA();
    if (strcmp(instruction.mnemonic, "TXS") == 0)
        TXS();
    if (strcmp(instruction.mnemonic, "TYA") == 0)
        TYA();
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

void drawZeroPage() {
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            printw("%02X ", memory.ram[i * 16 + j]);
        }
        printw("\n");
    }
    printw("\n");

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

void drawStatusBits() {
    mvprintw(0, 62, "C Z I D B R V N\n");
    mvprintw(1, 62, "%u", regs.SR.C);
    mvprintw(1, 64, "%u", regs.SR.Z);
    mvprintw(1, 66, "%u", regs.SR.I);
    mvprintw(1, 68, "%u", regs.SR.D);
    mvprintw(1, 70, "%u", regs.SR.B);
    mvprintw(1, 72, "- ");
    mvprintw(1, 74, "%u", regs.SR.V);
    mvprintw(1, 76, "%u\n", regs.SR.N);
}

void drawRegisters() {
    mvprintw(0, 50, " A: 0x%02X\n", regs.A);
    mvprintw(1, 50, " X: 0x%02X\n", regs.X);
    mvprintw(2, 50, " Y: 0x%02X\n", regs.Y);
    mvprintw(3, 50, "SP: 0x%02X\n", regs.SP);
    mvprintw(4, 50, "PC: 0x%04X\n", regs.PC);
    mvprintw(5, 50, "SR: 0x%02X\n\n", regs.SR.byte);
}

void displayRegisters(Registers regs) {
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

void NOP() {
    return;
}

void INX() {
    regs.X += 1;
}

void INY() {
    regs.Y += 1;
}

void DEX() {
    regs.X -= 1;
}

void DEY() {
    regs.Y -= 1;
}

void SEC(){
    regs.SR.C = 1;
}

void SED(){
    regs.SR.D = 1;
}

void SEI(){
    regs.SR.I = 1;
}

void CLC(){
    regs.SR.C = 0;
}

void CLD(){
    regs.SR.D = 0;
}

void CLI(){
    regs.SR.I = 0;
}

void CLV(){
    regs.SR.V = 0;    
}

void JSR(Instruction instruction) {
    u16 returnAddr = regs.PC + 2; // Only + 2 because RTS will automatically add 1.

    regs.SP--;
    memory.ram[0x100 + regs.SP] = (returnAddr >> 8) & 0xFF; // High byte

    regs.SP--;
    memory.ram[0x100 + regs.SP] = returnAddr & 0xFF; // Low byte
    
    regs.PC = instruction.operand.bytes; // Setting the PC
}

void RTS() {
    u16 returnAddr = memory.ram[regs.SP + 0x100]; // Pop low byte
    regs.SP++;

    returnAddr |= memory.ram[regs.SP + 0x100] << 8; // Pop high byte
    regs.SP++;

    regs.PC = returnAddr + 1;
}

void PHA() {
    memory.ram[0x100 + regs.SP] = regs.A;
    regs.SP--;
    regs.A = 0;
}

void PLA() {
    regs.SP++;
    regs.A = memory.ram[0x100 + regs.SP];

    UPDATE_Z_FLAG(regs.A);
    UPDATE_N_FLAG(regs.A);
}



void TAX() {
    regs.X = regs.A;
    UPDATE_Z_FLAG(regs.X);
    UPDATE_N_FLAG(regs.X);
}

void TAY() {
    regs.Y = regs.A;
    UPDATE_Z_FLAG(regs.Y);
    UPDATE_N_FLAG(regs.Y);
}

void TSX() {
    regs.X = regs.SP;
    UPDATE_Z_FLAG(regs.X);
    UPDATE_N_FLAG(regs.X);
}

void TXA() {
    regs.A = regs.X;
    UPDATE_Z_FLAG(regs.A);
    UPDATE_N_FLAG(regs.A);
}

void TXS() {
    regs.SP = regs.X;
}

void TYA() {
    regs.A = regs.Y;
    UPDATE_Z_FLAG(regs.A);
    UPDATE_N_FLAG(regs.A);
}
