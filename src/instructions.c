#include "6502.h"
#include "instructions.h"

extern Registers regs;
extern Memory memory;
extern InstructionMetaData imdLookup[256];

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

    imdLookup[0xA9] = (InstructionMetaData){"LDA", IMMEDIATE, 2, 2};
    imdLookup[0xA5] = (InstructionMetaData){"LDA", ZEROPAGE, 2, 3};
    imdLookup[0xB5] = (InstructionMetaData){"LDA", ZEROPAGE_X, 2, 4};
    imdLookup[0xAD] = (InstructionMetaData){"LDA", ABSOLUTE, 3, 4};
    imdLookup[0xBD] = (InstructionMetaData){"LDA", ABSOLUTE_X, 3, 4};
    imdLookup[0xB9] = (InstructionMetaData){"LDA", ABSOLUTE_Y, 3, 4};
    imdLookup[0xA1] = (InstructionMetaData){"LDA", X_INDIRECT, 2, 6};
    imdLookup[0xB1] = (InstructionMetaData){"LDA", INDIRECT_Y, 2, 5};


    imdLookup[0x85] = (InstructionMetaData){"STA", ZEROPAGE, 2, 3};
    imdLookup[0x95] = (InstructionMetaData){"STA", ZEROPAGE_X, 2, 4};
    imdLookup[0x8D] = (InstructionMetaData){"STA", ABSOLUTE, 3, 4};
    imdLookup[0x9D] = (InstructionMetaData){"STA", ABSOLUTE_X, 3, 5};
    imdLookup[0x99] = (InstructionMetaData){"STA", ABSOLUTE_Y, 3, 5};
    imdLookup[0x81] = (InstructionMetaData){"STA", X_INDIRECT, 2, 6};
    imdLookup[0x91] = (InstructionMetaData){"STA", INDIRECT_Y, 2, 6};

    // Custom instructions
    imdLookup[0x22] = (InstructionMetaData){"END", IMPLIED, 1, 0}; // Ends the program
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
    
    if (strcmp(instruction.mnemonic, "LDA") == 0)
        LDA(instruction);
    if (strcmp(instruction.mnemonic, "STA") == 0)
        STA(instruction);
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

void LDA(Instruction instruction) { 
    if (instruction.addressingMode == IMMEDIATE) { // A9
        regs.A = instruction.operand.lowByte;
    } else if (instruction.addressingMode == ZEROPAGE) { // A5
        regs.A = memory.ram[instruction.operand.lowByte];
    } else if (instruction.addressingMode == ZEROPAGE_X) { // B5
        u8 src = instruction.operand.lowByte + regs.X;
        regs.A = memory.ram[src];
    } else if (instruction.addressingMode == ABSOLUTE) { // AD
        u16 src = 0;
        src |= instruction.operand.lowByte;
        src |= (instruction.operand.highByte << 8);
        regs.A = memory.ram[src];
    } else if (instruction.addressingMode == ABSOLUTE_X) { // AD TODO: Special cycle case
        u16 src = 0;
        src |= instruction.operand.lowByte;
        src |= (instruction.operand.highByte << 8);
        src += regs.X;
        regs.A = memory.ram[src];
    } else if (instruction.addressingMode == ABSOLUTE_Y) { // AD TODO: Special cycle case
        u16 src = 0;
        src |= instruction.operand.lowByte;
        src |= (instruction.operand.highByte << 8);
        src += regs.Y;
        regs.A = memory.ram[src];
    } else if (instruction.addressingMode == X_INDIRECT) {
        u16 lookupAddr = (memory.ram[instruction.operand.lowByte + regs.X + 1] << 8) | memory.ram[instruction.operand.lowByte + regs.X];
        regs.A = (memory.ram[lookupAddr + 1] << 8) | memory.ram[lookupAddr];
    } else if (instruction.addressingMode == INDIRECT_Y) { // AD TODO: Special cycle case
        u16 lookupAddr = ((memory.ram[instruction.operand.lowByte + 1] << 8) | memory.ram[instruction.operand.lowByte]) + regs.Y;
        regs.A = (memory.ram[lookupAddr + 1] << 8) | memory.ram[lookupAddr];
    }

    UPDATE_Z_FLAG(regs.A);
    UPDATE_N_FLAG(regs.A);
}

void STA(Instruction instruction) {
    if (instruction.addressingMode == ZEROPAGE) { // 85
        memory.ram[instruction.operand.lowByte] = regs.A;
    } else if (instruction.addressingMode == ZEROPAGE_X) { // 95
        u8 dst = instruction.operand.lowByte + regs.X;
        memory.ram[dst] = regs.A;
    } else if (instruction.addressingMode == ABSOLUTE) { // 8D
        u16 dst = 0;
        dst |= instruction.operand.lowByte;
        dst |= (instruction.operand.highByte << 8);
        memory.ram[dst] = regs.A;
    } else if (instruction.addressingMode == ABSOLUTE_X) { // 8D
        u16 dst = 0;
        dst |= instruction.operand.lowByte;
        dst |= (instruction.operand.highByte << 8);
        dst += regs.X;
        memory.ram[dst] = regs.A;
    } else if (instruction.addressingMode == ABSOLUTE_Y) { // 8D
        u16 dst = 0;
        dst |= instruction.operand.lowByte;
        dst |= (instruction.operand.highByte << 8);
        dst += regs.Y;
        memory.ram[dst] = regs.A;
    } else if (instruction.addressingMode == X_INDIRECT) {
        u16 lookupAddr = (memory.ram[instruction.operand.lowByte + regs.X + 1] << 8) | memory.ram[instruction.operand.lowByte + regs.X];
        memory.ram[lookupAddr] = regs.A;
    } else if (instruction.addressingMode == INDIRECT_Y) {
        u16 lookupAddr = ((memory.ram[instruction.operand.lowByte + 1] << 8) | memory.ram[instruction.operand.lowByte]) + regs.Y;
        memory.ram[lookupAddr] = regs.A;
    }
}
