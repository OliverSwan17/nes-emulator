#include "6502.h"
#include "utils.h"

int main() {
    extern Registers regs;

    initMnemonicLookup();
    initAddressingModeLookup();
    initOperandCycles();
    initOperandLength();

    regs.A = 17;
    regs.X = 5;
    regs.Y = 6;
    regs.SP = 20;
    regs.SR.byte = 2;
    displayRegisters(regs);

    u8 binary = 0xCA;
    Instruction instruction = identifyInstruction(&binary);
    executeInstruction(instruction);
    executeInstruction(instruction);

    displayRegisters(regs);

    return 0;
}