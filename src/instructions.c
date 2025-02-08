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

    imdLookup[0xE0] = (InstructionMetaData){"CPX", IMMEDIATE, 2, 2};
    imdLookup[0xE4] = (InstructionMetaData){"CPX", ZEROPAGE, 2, 3};
    imdLookup[0xEC] = (InstructionMetaData){"CPX", ABSOLUTE, 3, 4};

    imdLookup[0xC0] = (InstructionMetaData){"CPY", IMMEDIATE, 2, 2};
    imdLookup[0xC4] = (InstructionMetaData){"CPY", ZEROPAGE, 2, 3};
    imdLookup[0xCC] = (InstructionMetaData){"CPY", ABSOLUTE, 3, 4};

    imdLookup[0xE6] = (InstructionMetaData){"INC", ZEROPAGE, 2, 5};
    imdLookup[0xF6] = (InstructionMetaData){"INC", ZEROPAGE_X, 2, 6};
    imdLookup[0xEE] = (InstructionMetaData){"INC", ABSOLUTE, 3, 6};
    imdLookup[0xFE] = (InstructionMetaData){"INC", ABSOLUTE_X, 3, 7};

    // Custom instructions
    imdLookup[0x22] = (InstructionMetaData){"END", IMPLIED, 1, 0}; // Ends the program
} // imdLookup[0x] = (InstructionMetaData){"", , , };

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

    if (strcmp(instruction.mnemonic, "CPX") == 0)
        CPX(instruction);
    
    if (strcmp(instruction.mnemonic, "CPY") == 0)
        CPY(instruction);
    
    if (strcmp(instruction.mnemonic, "INC") == 0)
        INC(instruction);

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
    u8 lowByte = instruction.operand.lowByte;
    u16 operand = instruction.operand.bytes;
    AddressingMode addrMode = instruction.addressingMode;
    u8 X = regs.X;
    u8 Y = regs.Y;
    
    if (addrMode == IMMEDIATE)
        regs.A = lowByte;
    else if (addrMode == ZEROPAGE)
        regs.A = READ_RAM(lowByte);
    else if (addrMode == ZEROPAGE_X)
        regs.A = READ_RAM(ZEROPAGE_X_ADDR(lowByte, X));
    else if (addrMode == ABSOLUTE)
        regs.A = READ_RAM(operand);
    else if (addrMode == ABSOLUTE_X) // TODO: Special cycle case
        regs.A = READ_RAM(ABSOLUTE_X_ADDR(operand, X));
    else if (addrMode == ABSOLUTE_Y) // TODO: Special cycle case
        regs.A = READ_RAM(ABSOLUTE_Y_ADDR(operand, Y));
    else if (addrMode == X_INDIRECT)
        regs.A = READ_LLHH_RAM(X_INDIRECT_ADDR(lowByte, X));
    else if (addrMode == INDIRECT_Y) // TODO: Special cycle case
        regs.A = READ_LLHH_RAM(INDIRECT_Y_ADDR(lowByte, Y)); 
    
    UPDATE_Z_FLAG(regs.A);
    UPDATE_N_FLAG(regs.A);
}

void STA(Instruction instruction) {
    u8 lowByte = instruction.operand.lowByte;
    u16 operand = instruction.operand.bytes;
    AddressingMode addrMode = instruction.addressingMode;
    u8 A = regs.A;
    u8 X = regs.X;
    u8 Y = regs.Y;

    if (addrMode == ZEROPAGE)
        WRITE_RAM(lowByte, A);
    else if (addrMode == ZEROPAGE_X)
        WRITE_RAM(ZEROPAGE_X_ADDR(lowByte, X), A);
    else if (addrMode == ABSOLUTE)
        WRITE_RAM(operand, A);
    else if (addrMode == ABSOLUTE_X)
        WRITE_RAM(ABSOLUTE_X_ADDR(operand, X), A);
    else if (addrMode == ABSOLUTE_Y) 
        WRITE_RAM(ABSOLUTE_Y_ADDR(operand, Y), A);
    else if (addrMode == X_INDIRECT)
        WRITE_RAM(X_INDIRECT_ADDR(lowByte, X), A);
    else if (addrMode == INDIRECT_Y)
        WRITE_RAM(INDIRECT_Y_ADDR(lowByte, Y), A);
}

void CPX(Instruction instruction) {
    AddressingMode addrMode = instruction.addressingMode;
    u8 M;
    u8 X = regs.X;
    
    if (addrMode == IMMEDIATE)
        M = instruction.operand.lowByte;
    else if (addrMode == ZEROPAGE)
        M = READ_RAM(instruction.operand.lowByte);
    else if (addrMode == ABSOLUTE)
        M = READ_RAM(instruction.operand.bytes);
    
    
    regs.SR.C = (X >= M) ? 1: 0;
    UPDATE_Z_FLAG(X - M);
    UPDATE_N_FLAG(M);
}

void CPY(Instruction instruction) {
    AddressingMode addrMode = instruction.addressingMode;
    u8 M;
    u8 Y = regs.Y;
    
    if (addrMode == IMMEDIATE)
        M = instruction.operand.lowByte;
    else if (addrMode == ZEROPAGE)
        M = READ_RAM(instruction.operand.lowByte);
    else if (addrMode == ABSOLUTE)
        M = READ_RAM(instruction.operand.bytes);
    
    
    regs.SR.C = (Y >= M) ? 1: 0;
    UPDATE_Z_FLAG(Y - M);
    UPDATE_N_FLAG(M);
}

void INC(Instruction instruction) {
    AddressingMode addrMode = instruction.addressingMode;
    u16 addr;

    if (addrMode == ZEROPAGE)
        addr = instruction.operand.lowByte;
    else if (addrMode == ZEROPAGE_X)
        addr = instruction.operand.lowByte + regs.X;
    else if (addrMode == ABSOLUTE)
        addr = instruction.operand.bytes;
    else if (addrMode == ABSOLUTE_X)
        addr = instruction.operand.bytes + regs.X;

    memory.ram[addr]++;
    u8 result = memory.ram[addr];
    
    UPDATE_Z_FLAG(result);
    UPDATE_N_FLAG(result);
}
