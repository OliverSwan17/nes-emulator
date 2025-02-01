#include "6502.h"

int main() {
    initInstructionMetaData();
    powerUp();

    /*
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
    */

    return 0;
}