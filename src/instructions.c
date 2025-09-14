#include "6502.h"
#include "instructions.h"

extern Registers regs;
extern Memory memory;
extern InstructionMetaData imdLookup[256];

// Mnemonic, Addressing Mode, Bytes, Cycles
void initInstructionMetaData() {
    for (int i = 0; i < 256; i++)
        imdLookup[i] = (InstructionMetaData){"NOP", IMPLIED, 1, 2};

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

    imdLookup[0xC6] = (InstructionMetaData){"DEC", ZEROPAGE, 2, 5};
    imdLookup[0xD6] = (InstructionMetaData){"DEC", ZEROPAGE_X, 2, 6};
    imdLookup[0xCE] = (InstructionMetaData){"DEC", ABSOLUTE, 3, 6};
    imdLookup[0xDE] = (InstructionMetaData){"DEC", ABSOLUTE_X, 3, 7};

    imdLookup[0x29] = (InstructionMetaData){"AND", IMMEDIATE, 2, 2};
    imdLookup[0x25] = (InstructionMetaData){"AND", ZEROPAGE, 2, 3};
    imdLookup[0x35] = (InstructionMetaData){"AND", ZEROPAGE_X, 2, 4};
    imdLookup[0x2D] = (InstructionMetaData){"AND", ABSOLUTE, 3, 4};
    imdLookup[0x3D] = (InstructionMetaData){"AND", ABSOLUTE_X, 3, 4};
    imdLookup[0x39] = (InstructionMetaData){"AND", ABSOLUTE_Y, 3, 4};
    imdLookup[0x21] = (InstructionMetaData){"AND", X_INDIRECT, 2, 6};
    imdLookup[0x31] = (InstructionMetaData){"AND", INDIRECT_Y, 2, 5};

    imdLookup[0x09] = (InstructionMetaData){"ORA", IMMEDIATE, 2, 2};
    imdLookup[0x05] = (InstructionMetaData){"ORA", ZEROPAGE, 2, 3};
    imdLookup[0x15] = (InstructionMetaData){"ORA", ZEROPAGE_X, 2, 4};
    imdLookup[0x0D] = (InstructionMetaData){"ORA", ABSOLUTE, 3, 4};
    imdLookup[0x1D] = (InstructionMetaData){"ORA", ABSOLUTE_X, 3, 4};
    imdLookup[0x19] = (InstructionMetaData){"ORA", ABSOLUTE_Y, 3, 4};
    imdLookup[0x01] = (InstructionMetaData){"ORA", X_INDIRECT, 2, 6};
    imdLookup[0x11] = (InstructionMetaData){"ORA", INDIRECT_Y, 2, 5};

    imdLookup[0x49] = (InstructionMetaData){"EOR", IMMEDIATE, 2, 2};
    imdLookup[0x45] = (InstructionMetaData){"EOR", ZEROPAGE, 2, 3};
    imdLookup[0x55] = (InstructionMetaData){"EOR", ZEROPAGE_X, 2, 4};
    imdLookup[0x4D] = (InstructionMetaData){"EOR", ABSOLUTE, 3, 4};
    imdLookup[0x5D] = (InstructionMetaData){"EOR", ABSOLUTE_X, 3, 4};
    imdLookup[0x59] = (InstructionMetaData){"EOR", ABSOLUTE_Y, 3, 4};
    imdLookup[0x41] = (InstructionMetaData){"EOR", X_INDIRECT, 2, 6};
    imdLookup[0x51] = (InstructionMetaData){"EOR", INDIRECT_Y, 2, 5};

    imdLookup[0xA2] = (InstructionMetaData){"LDX", IMMEDIATE, 2, 2};
    imdLookup[0xA6] = (InstructionMetaData){"LDX", ZEROPAGE, 2, 3};
    imdLookup[0xB6] = (InstructionMetaData){"LDX", ZEROPAGE_Y, 2, 4};
    imdLookup[0xAE] = (InstructionMetaData){"LDX", ABSOLUTE, 3, 4};
    imdLookup[0xBE] = (InstructionMetaData){"LDX", ABSOLUTE_Y, 3, 4};

    imdLookup[0xA0] = (InstructionMetaData){"LDY", IMMEDIATE, 2, 2};
    imdLookup[0xA4] = (InstructionMetaData){"LDY", ZEROPAGE, 2, 3};
    imdLookup[0xB4] = (InstructionMetaData){"LDY", ZEROPAGE_X, 2, 4};
    imdLookup[0xAC] = (InstructionMetaData){"LDY", ABSOLUTE, 3, 4};
    imdLookup[0xBC] = (InstructionMetaData){"LDY", ABSOLUTE_X, 3, 4};

    imdLookup[0x0A] = (InstructionMetaData){"ASL", ACCUMULATOR, 1, 2};
    imdLookup[0x06] = (InstructionMetaData){"ASL", ZEROPAGE, 2, 5};
    imdLookup[0x16] = (InstructionMetaData){"ASL", ZEROPAGE_X, 2, 6};
    imdLookup[0x0E] = (InstructionMetaData){"ASL", ABSOLUTE, 3, 6};
    imdLookup[0x1E] = (InstructionMetaData){"ASL", ABSOLUTE_X, 3, 7};

    imdLookup[0x4A] = (InstructionMetaData){"LSR", ACCUMULATOR, 1, 2};
    imdLookup[0x46] = (InstructionMetaData){"LSR", ZEROPAGE, 2, 5};
    imdLookup[0x56] = (InstructionMetaData){"LSR", ZEROPAGE_X, 2, 6};
    imdLookup[0x4E] = (InstructionMetaData){"LSR", ABSOLUTE, 3, 6};
    imdLookup[0x5E] = (InstructionMetaData){"LSR", ABSOLUTE_X, 3, 7};

    imdLookup[0x86] = (InstructionMetaData){"STX", ZEROPAGE, 2, 3};
    imdLookup[0x96] = (InstructionMetaData){"STX", ZEROPAGE_Y, 2, 4};
    imdLookup[0x8E] = (InstructionMetaData){"STX", ABSOLUTE, 3, 4};

    imdLookup[0x84] = (InstructionMetaData){"STY", ZEROPAGE, 2, 3};
    imdLookup[0x94] = (InstructionMetaData){"STY", ZEROPAGE_X, 2, 4};
    imdLookup[0x8C] = (InstructionMetaData){"STY", ABSOLUTE, 3, 4};

    imdLookup[0x2A] = (InstructionMetaData){"ROL", ACCUMULATOR, 1, 2};
    imdLookup[0x26] = (InstructionMetaData){"ROL", ZEROPAGE, 2, 5};
    imdLookup[0x36] = (InstructionMetaData){"ROL", ZEROPAGE_X, 2, 6};
    imdLookup[0x2E] = (InstructionMetaData){"ROL", ABSOLUTE, 3, 6};
    imdLookup[0x3E] = (InstructionMetaData){"ROL", ABSOLUTE_X, 3, 7};

    imdLookup[0x6A] = (InstructionMetaData){"ROR", ACCUMULATOR, 1, 2};
    imdLookup[0x66] = (InstructionMetaData){"ROR", ZEROPAGE, 2, 5};
    imdLookup[0x76] = (InstructionMetaData){"ROR", ZEROPAGE_X, 2, 6};
    imdLookup[0x6E] = (InstructionMetaData){"ROR", ABSOLUTE, 3, 6};
    imdLookup[0x7E] = (InstructionMetaData){"ROR", ABSOLUTE_X, 3, 7};

    imdLookup[0x4C] = (InstructionMetaData){"JMP", ABSOLUTE, 3, 3};
    imdLookup[0x6C] = (InstructionMetaData){"JMP", INDIRECT, 3, 5};

    imdLookup[0xC9] = (InstructionMetaData){"CMP", IMMEDIATE, 2, 2};
    imdLookup[0xC5] = (InstructionMetaData){"CMP", ZEROPAGE, 2, 3};
    imdLookup[0xD5] = (InstructionMetaData){"CMP", ZEROPAGE_X, 2, 4};
    imdLookup[0xCD] = (InstructionMetaData){"CMP", ABSOLUTE, 3, 4};
    imdLookup[0xDD] = (InstructionMetaData){"CMP", ABSOLUTE_X, 3, 4};
    imdLookup[0xD9] = (InstructionMetaData){"CMP", ABSOLUTE_Y, 3, 4};
    imdLookup[0xC1] = (InstructionMetaData){"CMP", X_INDIRECT, 2, 6};
    imdLookup[0xD1] = (InstructionMetaData){"CMP", INDIRECT_Y, 2, 5};

    imdLookup[0x24] = (InstructionMetaData){"BIT", ZEROPAGE, 2, 3};
    imdLookup[0x2C] = (InstructionMetaData){"BIT", ABSOLUTE, 3, 4};

    imdLookup[0x08] = (InstructionMetaData){"PHP", IMPLIED, 1, 3};
    imdLookup[0x28] = (InstructionMetaData){"PLP", IMPLIED, 1, 4};

    imdLookup[0x90] = (InstructionMetaData){"BCC", RELATIVE, 2, 2};
    imdLookup[0xB0] = (InstructionMetaData){"BCS", RELATIVE, 2, 2};
    imdLookup[0xF0] = (InstructionMetaData){"BEQ", RELATIVE, 2, 2};
    imdLookup[0x30] = (InstructionMetaData){"BMI", RELATIVE, 2, 2};
    imdLookup[0xD0] = (InstructionMetaData){"BNE", RELATIVE, 2, 2};
    imdLookup[0x10] = (InstructionMetaData){"BPL", RELATIVE, 2, 2};
    imdLookup[0x50] = (InstructionMetaData){"BVC", RELATIVE, 2, 2};
    imdLookup[0x70] = (InstructionMetaData){"BVS", RELATIVE, 2, 2};

    imdLookup[0x00] = (InstructionMetaData){"BRK", IMPLIED, 2, 7};
    imdLookup[0x40] = (InstructionMetaData){"RTI", IMPLIED, 1, 6};

    imdLookup[0x69] = (InstructionMetaData){"ADC", IMMEDIATE, 2, 2};
    imdLookup[0x65] = (InstructionMetaData){"ADC", ZEROPAGE, 2, 3};
    imdLookup[0x75] = (InstructionMetaData){"ADC", ZEROPAGE_X, 2, 4};
    imdLookup[0x6D] = (InstructionMetaData){"ADC", ABSOLUTE, 3, 4};
    imdLookup[0x7D] = (InstructionMetaData){"ADC", ABSOLUTE_X, 3, 4};
    imdLookup[0x79] = (InstructionMetaData){"ADC", ABSOLUTE_Y, 3, 4};
    imdLookup[0x61] = (InstructionMetaData){"ADC", X_INDIRECT, 2, 6};
    imdLookup[0x71] = (InstructionMetaData){"ADC", INDIRECT_Y, 2, 5};

    imdLookup[0xE9] = (InstructionMetaData){"SBC", IMMEDIATE, 2, 2};
    imdLookup[0xE5] = (InstructionMetaData){"SBC", ZEROPAGE, 2, 3};
    imdLookup[0xF5] = (InstructionMetaData){"SBC", ZEROPAGE_X, 2, 4};
    imdLookup[0xED] = (InstructionMetaData){"SBC", ABSOLUTE, 3, 4};
    imdLookup[0xFD] = (InstructionMetaData){"SBC", ABSOLUTE_X, 3, 4};
    imdLookup[0xF9] = (InstructionMetaData){"SBC", ABSOLUTE_Y, 3, 4};
    imdLookup[0xE1] = (InstructionMetaData){"SBC", X_INDIRECT, 2, 6};
    imdLookup[0xF1] = (InstructionMetaData){"SBC", INDIRECT_Y, 2, 5};

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
    
    if (strcmp(instruction.mnemonic, "DEC") == 0)
        DEC(instruction);
    
    if (strcmp(instruction.mnemonic, "AND") == 0)
        AND(instruction);
    if (strcmp(instruction.mnemonic, "ORA") == 0)
        ORA(instruction);
    if (strcmp(instruction.mnemonic, "EOR") == 0)
        EOR(instruction);
    
    if (strcmp(instruction.mnemonic, "LDX") == 0)
        LDX(instruction);
    if (strcmp(instruction.mnemonic, "LDY") == 0)
        LDY(instruction);
    
    if (strcmp(instruction.mnemonic, "ASL") == 0)
        ASL(instruction);
    if (strcmp(instruction.mnemonic, "LSR") == 0)
        LSR(instruction);
    if (strcmp(instruction.mnemonic, "ROL") == 0)
        ROL(instruction);
    if (strcmp(instruction.mnemonic, "ROR") == 0)
        ROR(instruction);

    if (strcmp(instruction.mnemonic, "STX") == 0)
        STX(instruction);
    if (strcmp(instruction.mnemonic, "STY") == 0)
        STY(instruction);
    
    if (strcmp(instruction.mnemonic, "JMP") == 0)
        JMP(instruction);
    
    if (strcmp(instruction.mnemonic, "CMP") == 0)
        CMP(instruction);

    if (strcmp(instruction.mnemonic, "BIT") == 0)
        BIT(instruction);
    
        
    if (strcmp(instruction.mnemonic, "PHP") == 0)
        PHP();

    if (strcmp(instruction.mnemonic, "PLP") == 0)
        PLP();
    

    if (strcmp(instruction.mnemonic, "BCC") == 0)
        BCC(instruction);
    if (strcmp(instruction.mnemonic, "BCS") == 0)
        BCS(instruction);
    if (strcmp(instruction.mnemonic, "BEQ") == 0)
        BEQ(instruction);
    if (strcmp(instruction.mnemonic, "BMI") == 0)
        BMI(instruction);
    if (strcmp(instruction.mnemonic, "BNE") == 0)
        BNE(instruction);
    if (strcmp(instruction.mnemonic, "BPL") == 0)
        BPL(instruction);
    if (strcmp(instruction.mnemonic, "BVC") == 0)
        BVC(instruction);
    if (strcmp(instruction.mnemonic, "BVS") == 0)
        BVS(instruction);
    
    if (strcmp(instruction.mnemonic, "BRK") == 0)
        BRK(instruction);
    
    if (strcmp(instruction.mnemonic, "RTI") == 0)
        RTI(instruction);
    
    if (strcmp(instruction.mnemonic, "ADC") == 0)
        ADC(instruction);
    if (strcmp(instruction.mnemonic, "SBC") == 0)
        SBC(instruction);

}

void NOP() {
    return;
}

void INX() {
    regs.X += 1;
    UPDATE_N_FLAG(regs.X);
    UPDATE_Z_FLAG(regs.X);
}

void INY() {
    regs.Y += 1;
    UPDATE_N_FLAG(regs.Y);
    UPDATE_Z_FLAG(regs.Y);
}

void DEX() {
    regs.X -= 1;
    UPDATE_N_FLAG(regs.X);
    UPDATE_Z_FLAG(regs.X);
}

void DEY() {
    regs.Y -= 1;
    UPDATE_N_FLAG(regs.Y);
    UPDATE_Z_FLAG(regs.Y);
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

    memory.ram[0x100 + regs.SP] = (returnAddr >> 8) & 0xFF; // High byte
    regs.SP--;

    
    memory.ram[0x100 + regs.SP] = returnAddr & 0xFF; // Low byte
    regs.SP--;
    
    regs.PC = instruction.operand.bytes; // Setting the PC
}

void RTS() {
    regs.SP++;
    u16 returnAddr = memory.ram[regs.SP + 0x100]; // Pop low byte
    
    regs.SP++;
    returnAddr |= memory.ram[regs.SP + 0x100] << 8; // Pop high byte

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
        regs.A = READ_RAM((ZEROPAGE_X_ADDR(lowByte, X)));
    else if (addrMode == ABSOLUTE)
        regs.A = READ_RAM(operand);
    else if (addrMode == ABSOLUTE_X) // TODO: Special cycle case
        regs.A = READ_RAM(ABSOLUTE_X_ADDR(operand, X));
    else if (addrMode == ABSOLUTE_Y) // TODO: Special cycle case
        regs.A = READ_RAM(ABSOLUTE_Y_ADDR(operand, Y));
    else if (addrMode == X_INDIRECT)
        regs.A = READ_RAM(X_INDIRECT_ADDR(lowByte, X));
    else if (addrMode == INDIRECT_Y) // TODO: Special cycle case
        regs.A = READ_RAM(INDIRECT_Y_ADDR(lowByte, Y)); 
    
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
    
    
    regs.SR.C = (X >= M);
    regs.SR.Z = (X == M);
    UPDATE_N_FLAG(X - M);
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
    
    
    regs.SR.C = (Y >= M);
    regs.SR.Z = (Y == M);
    UPDATE_N_FLAG(Y - M);
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

void DEC(Instruction instruction) {
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

    memory.ram[addr]--;
    u8 result = memory.ram[addr];
    
    UPDATE_Z_FLAG(result);
    UPDATE_N_FLAG(result);
}

void AND(Instruction instruction) {
    Bitwise(instruction);
}

void ORA(Instruction instruction) {
    Bitwise(instruction);
}

void EOR(Instruction instruction) {
    Bitwise(instruction);
}

void Bitwise(Instruction instruction) {
    AddressingMode addrMode = instruction.addressingMode;
    u16 operand = instruction.operand.bytes;
    u8 lowByte = instruction.operand.lowByte;
    u16 addr = 0;
    u8 value = 0;
    char *mnemonic = instruction.mnemonic;

    if (addrMode == IMMEDIATE) {
        if (strcmp(mnemonic, "AND") == 0)
            regs.A &= lowByte;
        else if (strcmp(mnemonic, "ORA") == 0)
            regs.A |= lowByte;
        else if (strcmp(mnemonic, "EOR") == 0)
            regs.A ^= lowByte;

        UPDATE_Z_FLAG(regs.A);
        UPDATE_N_FLAG(regs.A);
        return;
    }

    if (addrMode == ZEROPAGE)
        addr = lowByte;
    else if (addrMode == ZEROPAGE_X)
        addr = ZEROPAGE_X_ADDR(lowByte, regs.X);
    else if (addrMode == ABSOLUTE)
        addr = operand;
    else if (addrMode == ABSOLUTE_X)
        addr = ABSOLUTE_X_ADDR(operand, regs.X);
    else if (addrMode == ABSOLUTE_Y)
        addr = ABSOLUTE_Y_ADDR(operand, regs.Y);
    else if (addrMode == X_INDIRECT)
        addr = X_INDIRECT_ADDR(lowByte, regs.X);
    else if (addrMode == INDIRECT_Y)
        addr = INDIRECT_Y_ADDR(lowByte, regs.Y);

    value = READ_RAM(addr);

    if (strcmp(mnemonic, "AND") == 0)
        regs.A &= value;
    else if (strcmp(mnemonic, "ORA") == 0)
        regs.A |= value;
    else if (strcmp(mnemonic, "EOR") == 0)
        regs.A ^= value;
    
    UPDATE_Z_FLAG(regs.A);
    UPDATE_N_FLAG(regs.A);
}

void LDX(Instruction instruction) {
    AddressingMode addrMode = instruction.addressingMode;
    u16 operand = instruction.operand.bytes;
    u8 lowByte = instruction.operand.lowByte;

    if (addrMode == IMMEDIATE)
        regs.X = lowByte;
    else if (addrMode == ZEROPAGE)
        regs.X = READ_RAM(lowByte);
    else if (addrMode == ZEROPAGE_Y)
        regs.X = READ_RAM(ZEROPAGE_Y_ADDR(lowByte, regs.Y));
    else if (addrMode == ABSOLUTE)
        regs.X = READ_RAM(operand);
    else if (addrMode == ABSOLUTE_Y)
        regs.X = READ_RAM(ABSOLUTE_Y_ADDR(operand, regs.Y));

    UPDATE_Z_FLAG(regs.X);
    UPDATE_N_FLAG(regs.X);
}

void LDY(Instruction instruction) {
    AddressingMode addrMode = instruction.addressingMode;
    u16 operand = instruction.operand.bytes;
    u8 lowByte = instruction.operand.lowByte;

    if (addrMode == IMMEDIATE)
        regs.Y = lowByte;
    else if (addrMode == ZEROPAGE)
        regs.Y = READ_RAM(lowByte);
    else if (addrMode == ZEROPAGE_X)
        regs.Y = READ_RAM(ZEROPAGE_X_ADDR(lowByte, regs.X));
    else if (addrMode == ABSOLUTE)
        regs.Y = READ_RAM(operand);
    else if (addrMode == ABSOLUTE_X)
        regs.Y = READ_RAM(ABSOLUTE_X_ADDR(operand, regs.X));

    UPDATE_Z_FLAG(regs.Y);
    UPDATE_N_FLAG(regs.Y);
}

void ASL(Instruction instruction) {
    AddressingMode addrMode = instruction.addressingMode;
    u16 operand = instruction.operand.bytes;
    u8 lowByte = instruction.operand.lowByte;
    u8 value = 0;
    u16 effAddr= 0;

    if (addrMode == ACCUMULATOR)
        value = regs.A;
    else {
        if (addrMode == ZEROPAGE)
            effAddr = lowByte;
        else if (addrMode == ZEROPAGE_X)
            effAddr = lowByte + regs.X;
        else if (addrMode == ABSOLUTE)
            effAddr = operand;
        else if (addrMode == ABSOLUTE_X)
            effAddr = operand + regs.X;
        
        value = READ_RAM(effAddr);
    }
   
    u8 result = value << 1;

    if (addrMode == ACCUMULATOR)
        regs.A = result;
    else
        WRITE_RAM(effAddr, result);

    regs.SR.C = (value >> 7);
    UPDATE_Z_FLAG(result);
    UPDATE_N_FLAG(result);
}

void LSR(Instruction instruction) {
    AddressingMode addrMode = instruction.addressingMode;
    u16 operand = instruction.operand.bytes;
    u8 lowByte = instruction.operand.lowByte;
    u8 value = 0;
    u16 effAddr= 0;

    if (addrMode == ACCUMULATOR)
        value = regs.A;
    else {
        if (addrMode == ZEROPAGE)
            effAddr = lowByte;
        else if (addrMode == ZEROPAGE_X)
            effAddr = lowByte + regs.X;
        else if (addrMode == ABSOLUTE)
            effAddr = operand;
        else if (addrMode == ABSOLUTE_X)
            effAddr = operand + regs.X;
        
        value = READ_RAM(effAddr);
    }
   
    u8 result = value >> 1;

    if (addrMode == ACCUMULATOR)
        regs.A = result;
    else
        WRITE_RAM(effAddr, result);

    regs.SR.C = value & 0b1;
    UPDATE_Z_FLAG(result);
    regs.SR.N = 0;
}

void STX(Instruction instruction) {
    AddressingMode addrMode = instruction.addressingMode;
    u8 lowByte = instruction.operand.lowByte;

    if (addrMode == ZEROPAGE)
        WRITE_RAM(lowByte, regs.X);
    else if (addrMode == ZEROPAGE_Y)
        WRITE_RAM(ZEROPAGE_Y_ADDR(lowByte, regs.Y), regs.X);
    else if (addrMode == ABSOLUTE)
        WRITE_RAM(instruction.operand.bytes, regs.X);
}

void STY(Instruction instruction) {
    AddressingMode addrMode = instruction.addressingMode;
    u16 operand = instruction.operand.bytes;
    u8 lowByte = instruction.operand.lowByte;

    if (addrMode == ZEROPAGE)
        WRITE_RAM(lowByte, regs.Y);
    else if (addrMode == ZEROPAGE_X)
        WRITE_RAM(ZEROPAGE_X_ADDR(lowByte, regs.X), regs.Y);
    else if (addrMode == ABSOLUTE)
        WRITE_RAM(operand, regs.Y);
}

void ROL(Instruction instruction) {
    AddressingMode addrMode = instruction.addressingMode;
    u16 operand = instruction.operand.bytes;
    u8 lowByte = instruction.operand.lowByte;

    u8 value = 0;
    u8 result = 0;
    u16 effAddr = 0;

    if (addrMode == ACCUMULATOR) 
        value = regs.A;
    else if (addrMode == ZEROPAGE)
        effAddr = lowByte;
    else if (addrMode == ZEROPAGE_X)
        effAddr = lowByte + regs.X;
    else if (addrMode == ABSOLUTE)
        effAddr = operand;
    else if (addrMode == ABSOLUTE_X)
        effAddr = operand + regs.X;
    
    if (addrMode != ACCUMULATOR)
        value = READ_RAM(effAddr);

    result = (value << 1) | (regs.SR.C);

    if (addrMode == ACCUMULATOR)
        regs.A = result;
    else
        WRITE_RAM(effAddr, result);

    regs.SR.C = (value >> 7);
    UPDATE_Z_FLAG(result);
    UPDATE_N_FLAG(result);
}

void ROR(Instruction instruction) {
    AddressingMode addrMode = instruction.addressingMode;
    u16 operand = instruction.operand.bytes;
    u8 lowByte = instruction.operand.lowByte;

    u8 value = 0;
    u8 result = 0;
    u16 effAddr = 0;

    if (addrMode == ACCUMULATOR) 
        value = regs.A;
    else if (addrMode == ZEROPAGE)
        effAddr = lowByte;
    else if (addrMode == ZEROPAGE_X)
        effAddr = lowByte + regs.X;
    else if (addrMode == ABSOLUTE)
        effAddr = operand;
    else if (addrMode == ABSOLUTE_X)
        effAddr = operand + regs.X;
    
    if (addrMode != ACCUMULATOR)
        value = READ_RAM(effAddr);

    result = (value >> 1) | (regs.SR.C << 7);

    if (addrMode == ACCUMULATOR)
        regs.A = result;
    else
        WRITE_RAM(effAddr, result);

    regs.SR.C = value & 0b1;
    UPDATE_Z_FLAG(result);
    UPDATE_N_FLAG(result);
}

void JMP(Instruction instruction) {
    AddressingMode addrMode = instruction.addressingMode;
    u16 operand = instruction.operand.bytes;

    if (addrMode == ABSOLUTE) {
        regs.PC = operand;
    } else if (addrMode == INDIRECT) {
        // 6502 page boundary bug: if low byte of indirect address is 0xFF,
        // the high byte is read from the same page instead of crossing to next page
        if ((operand & 0xFF) == 0xFF) {
            regs.PC = (memory.ram[operand & 0xFF00] << 8) | memory.ram[operand];
        } else {
            regs.PC = (memory.ram[operand + 1] << 8) | memory.ram[operand];
        }
    }
}

void CMP(Instruction instruction) {
    AddressingMode addrMode = instruction.addressingMode;
    u16 operand = instruction.operand.bytes;
    u8 lowByte = instruction.operand.lowByte;
    u8 M = 0;

    if (addrMode == IMMEDIATE)
        M = lowByte;
    else if (addrMode == ZEROPAGE)
        M = READ_RAM(lowByte);
    else if (addrMode == ZEROPAGE_X)
        M = READ_RAM(lowByte + regs.X);
    else if (addrMode == ABSOLUTE)
        M = READ_RAM(operand);
    else if (addrMode == ABSOLUTE_X) // TODO: Special cycle case
        M = READ_RAM(operand + regs.X);
    else if (addrMode == ABSOLUTE_Y) // TODO: Special cycle case
        M = READ_RAM(operand + regs.Y);
    else if (addrMode == X_INDIRECT)
        M = READ_WORD_ZEROPAGE(X_INDIRECT_ADDR(lowByte, regs.X));
    else if (addrMode == INDIRECT_Y) // TODO: Special cycle case
        M = READ_WORD_ZEROPAGE(INDIRECT_Y_ADDR(lowByte, regs.Y));

    u8 result = regs.A - M;
    regs.SR.Z = (result == 0);
    regs.SR.N = (result >> 7);
    regs.SR.C = (regs.A >= M);
}

void BIT(Instruction instruction) {
    AddressingMode addrMode = instruction.addressingMode;
    u16 operand = instruction.operand.bytes;
    u8 M = 0;

    if (addrMode == ZEROPAGE)
        M = READ_RAM(instruction.operand.lowByte);
    else if (addrMode == ABSOLUTE)
        M = READ_RAM(operand);
    
    regs.SR.N = M >> 7;
    regs.SR.V = (M >> 6) & 1;
    regs.SR.Z = ((regs.A & M) == 0); 
}

void PHP() {
    WRITE_RAM(0x100 + regs.SP, regs.SR.byte | (1 << 4) | (1 << 5));
    regs.SP--;
}

void PLP() {
    regs.SP++;
    u8 result = READ_RAM(0x100 + regs.SP);
    regs.SR.C = result & 0b1;
    regs.SR.Z = (result >> 1) & 0b1;
    regs.SR.I = (result >> 2) & 0b1;
    regs.SR.D = (result >> 3) & 0b1;
    regs.SR.V = (result >> 6) & 0b1;
    regs.SR.N = (result >> 7) & 0b1;
}



void BCC(Instruction instruction) {
    if (regs.SR.C == 0)
        regs.PC += (s8) instruction.operand.lowByte;
}

void BCS(Instruction instruction) {
    if (regs.SR.C)
    regs.PC += (s8) instruction.operand.lowByte;
}

void BEQ(Instruction instruction) {
    if (regs.SR.Z)
        regs.PC += (s8) instruction.operand.lowByte;
}

void BMI(Instruction instruction) {
    if (regs.SR.N)
        regs.PC += (s16)(s8) instruction.operand.lowByte;
}

void BNE(Instruction instruction) {
    if (regs.SR.Z == 0)
        regs.PC += (s8) instruction.operand.lowByte;
}

void BPL(Instruction instruction) {
    if (regs.SR.N == 0)
        regs.PC += (s8) instruction.operand.lowByte;
}

void BVC(Instruction instruction) {
    if (regs.SR.V == 0)
        regs.PC += (s8) instruction.operand.lowByte;
}

void BVS(Instruction instruction) {
    if (regs.SR.V)
        regs.PC += (s8) instruction.operand.lowByte;
}

void BRK() {
    WRITE_RAM(0x100 + regs.SP, ((regs.PC + 2) >> 8) & 0xFF);
    regs.SP--;
    WRITE_RAM(0x100 + regs.SP, (regs.PC + 2) & 0xFF);
    regs.SP--;
    WRITE_RAM(0x100 + regs.SP, regs.SR.byte | (1 << 5) | (1 << 4)); // Set unused bit (5) and break bit (4)
    regs.SP--;
    regs.SR.I = 1;
    regs.PC = READ_WORD_ABSOLUTE(0xFFFE);
}

void RTI() {
    regs.SP++;
    regs.SR.byte = (READ_RAM(0x100 + regs.SP) & (~(1 << 4))) | ((1 << 5)); // Ignoring break and unused bit.

    regs.SP++;
    u16 returnAddr = memory.ram[regs.SP + 0x100]; // Pop low byte

    regs.SP++;
    returnAddr |= memory.ram[regs.SP + 0x100] << 8; // Pop high byte

    regs.PC = returnAddr;
}

void ADC(Instruction instruction) {
    AddressingMode addrMode = instruction.addressingMode;
    u16 operand = instruction.operand.bytes;
    u8 lowByte = instruction.operand.lowByte;
    u8 M = 0;
    u8 A = regs.A;

    if (addrMode == IMMEDIATE)
        M = lowByte;
    else if (addrMode == ZEROPAGE)
        M = READ_RAM(lowByte);
    else if (addrMode == ZEROPAGE_X)
        M = READ_RAM(lowByte + regs.X);
    else if (addrMode == ABSOLUTE)
        M = READ_RAM(operand);
    else if (addrMode == ABSOLUTE_X) // TODO: Special cycle case
        M = READ_RAM(operand + regs.X);
    else if (addrMode == ABSOLUTE_Y) // TODO: Special cycle case
        M = READ_RAM(operand + regs.Y);
    else if (addrMode == X_INDIRECT)
        M = READ_WORD_ZEROPAGE(X_INDIRECT_ADDR(lowByte, regs.X));
    else if (addrMode == INDIRECT_Y) // TODO: Special cycle case
        M = READ_WORD_ZEROPAGE(INDIRECT_Y_ADDR(lowByte, regs.Y));
    
    s8 signedResult = (s8) A + (s8) M;
    u8 V = ((A >> 7) == (M >> 7)) && ((signedResult >> 7) != (A >> 7));

    u8 C = ((u16) A + (u16) M + (u16) regs.SR.C) > 255;

    u8 result = A + M + regs.SR.C;

    regs.A = result;

    UPDATE_N_FLAG(result);
    UPDATE_Z_FLAG(result);
    regs.SR.C = C;
    regs.SR.V = V;
}

void SBC(Instruction instruction) {
    AddressingMode addrMode = instruction.addressingMode;
    u16 operand = instruction.operand.bytes;
    u8 lowByte = instruction.operand.lowByte;
    u8 M = 0;
    u8 A = regs.A;

    if (addrMode == IMMEDIATE)
        M = lowByte;
    else if (addrMode == ZEROPAGE)
        M = READ_RAM(lowByte);
    else if (addrMode == ZEROPAGE_X)
        M = READ_RAM(lowByte + regs.X);
    else if (addrMode == ABSOLUTE)
        M = READ_RAM(operand);
    else if (addrMode == ABSOLUTE_X) // TODO: Special cycle case
        M = READ_RAM(operand + regs.X);
    else if (addrMode == ABSOLUTE_Y) // TODO: Special cycle case
        M = READ_RAM(operand + regs.Y);
    else if (addrMode == X_INDIRECT)
        M = READ_WORD_ZEROPAGE(X_INDIRECT_ADDR(lowByte, regs.X));
    else if (addrMode == INDIRECT_Y) // TODO: Special cycle case
        M = READ_WORD_ZEROPAGE(INDIRECT_Y_ADDR(lowByte, regs.Y));
    
    s16 signedResult = (s16) A - (s16) M - (1 - regs.SR.C);
    u8 V = ((A >> 7) != (M >> 7)) && ((signedResult >> 7) != (A >> 7));

    u8 C = ((s16) A - (s16) M - (1 - regs.SR.C)) >= 0;

    u8 result = A - M - (1 - regs.SR.C);

    regs.A = result;

    UPDATE_N_FLAG(result);
    UPDATE_Z_FLAG(result);
    regs.SR.C = C;
    regs.SR.V = V;
}
