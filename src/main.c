#include "6502.h"
#include "utils.h"

int main() {
    extern Registers regs;

    initInstructionMetaData();

    regs.A = 17;
    regs.X = 5;
    regs.Y = 6;
    regs.SP = 20;
    regs.SR.byte = 2;
    displayRegisters(regs);

    u8 binary[] = {0xCA}; // Decrement X
    Instruction instruction = identifyInstruction(binary);
    printInstruction(instruction);
    printf("------------------------------------------------\n");
    printf("X before DEX: %u\n", regs.X);
    executeInstruction(instruction);
    executeInstruction(instruction);
    printf("X after DEX: %u\n", regs.X);


    u8 binary2[] = {0x20, 0xCD, 0xAB};
    instruction = identifyInstruction(binary2);
    executeInstruction(instruction);
    printInstruction(instruction);

    return 0;
}