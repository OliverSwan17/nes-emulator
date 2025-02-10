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

    //u8 code[] = {0xA9, 0x17, 0x85, 0x21, 0x48, 0xA9, 0x00, 0xA5, 0x21, 0xA9, 0x00, 0x68, 0x22}; // Subroutines, stack, LDA, STA.


    //u8 code[] = {0xA9, 0x17, 0x85, 0x05, 0xE8, 0xA9, 0x00, 0xB5, 0x04, 0x22}; // Testing LDA zeropage_x 

    //u8 code[] = {0xA9, 0x17, 0x8D, 0x0F, 0x01, 0xA9, 0x00, 0xAD, 0x0F, 0x01, 0x22}; // Testing LDA and STA absolute addressing

    // This tests (indirect, x) addressing.
    //u8 code[] = {0xA9, 0x69, 0x85, 0x21, 0xA9, 0x17, 0x85, 0x69, 0xA9, 0x00, 0xE8, 0xE8, 0xE8, 0xA1, 0x1E, 0x22};
    /*
    LDA #0x69
    STA 0x21
    LDA #0x17
    STA 0x69
    LDA #0x00
    INX
    INX
    INX
    LDA (14,X)
    */

    // This test (indirect),y addressing.
    //u8 code[] = {0xA9, 0x45, 0x85, 0x09, 0xA9, 0x17, 0x85, 0x49, 0xA9, 0x04, 0xA8, 0xA9, 0x00, 0xB1, 0x09, 0x22};
    /*
    LDA #$45
    STA $09
    LDA #$17
    STA $49
    LDA #$04
    TAY
    LDA #$00
    LDA ($09),Y
    */

   // Testing sta (indirect, x) addressing
   //u8 code[] = {0xA9, 0x21, 0x85, 0x42, 0xA9, 0x17, 0xE8, 0xE8, 0xE8, 0x81, 0x3F, 0x22};

    // Testing cpx / cpy
    //u8 code[] = {0xA9, 0x17, 0x85, 0x17, 0xA9, 0x05, 0xAA, 0xE4, 0x17, 0xA9, 0x17, 0xAA, 0xE4, 0x17, 0xA9, 0x04, 0xAA, 0xE0, 0x04, 0xE0, 0x05, 0x22};


    // Testing INC for Zeropage and Zeropage, X
    //u8 code[] = {0xA9, 0x42, 0x85, 0x17, 0xE6, 0x17, 0xA9, 0x03, 0xAA, 0xF6, 0x14, 0x22};
    /*
    LDA #$42 
    STA $17
    INC $17
    LDA #$3
    TAX
    INC $14,X
    */

    // Testing INC for Absolute and absolute, X
    //u8 code[] = {0xA9, 0x42, 0x8D, 0x07, 0x01, 0xEE, 0x07, 0x01, 0xA9, 0x03, 0xAA, 0xFE, 0x04, 0x01, 0x22};
    /*
    LDA #$42
    STA $107
    INC $107
    LDA #$3
    TAX
    INC $104,X
    */

    // Testing AND / ORA / EOR  Modifying this opcode: v
    //u8 code[] = {0xA9, 0x53, 0x85, 0x21, 0xA9, 0x78, 0x45, 0x21, 0x22};
    /*
    LDA #$17
    STA $21
    LDA #$05
    AND $21
    */

    // Testing LDX and LDY
    //u8 code[] = {0xA2, 0x17, 0xA0, 0xFE, 0xA9, 0x49, 0x8D, 0xEC, 0x01, 0xAE, 0xEC, 0x01, 0xAC, 0xEC, 0x01, 0x22};
    /*
    LDX #$17
    LDY #$FE
    LDA #$49
    STA $1EC
    LDX $1EC
    LDY $1EC
    */

    // Testing ASL / LSR
    //u8 code[] = {0xA2, 0x07, 0xA9, 0x17, 0x0A, 0xA9, 0x17, 0x85, 0x17, 0x06, 0x17, 0x8D, 0x27, 0x01, 0x0E, 0x27, 0x01, 0x8D, 0x27, 0x01, 0x1E, 0x20, 0x01, 0x85, 0x17, 0x16, 0x10, 0x22};
    //u8 code[] = {0xA2, 0x07, 0xA9, 0x17, 0x4A, 0xA9, 0x17, 0x85, 0x17, 0x46, 0x17, 0x8D, 0x27, 0x01, 0x4E, 0x27, 0x01, 0x8D, 0x27, 0x01, 0x5E, 0x20, 0x01, 0x85, 0x17, 0x56, 0x10, 0x22};
    /*
    LDX #$7
    LDA #$17
    ASL A
    LDA #$17
    STA $17
    ASL $17
    STA $127
    ASL $127
    STA $127
    ASL $120,X
    STA $17
    ASL $10,X
    */
   
    // Testing ROL and ROR
    u8 code[] = {0xA9, 0xFF, 0x2A, 0xA9, 0x00, 0x2A, 0xA9, 0x01, 0x6A, 0x22};
    /*
    LDA #$FF
    ROL A
    LDA #$43
    ROL A
    LDA #$01
    ROR
*/

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
