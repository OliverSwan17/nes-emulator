#include "utils.h"

void displayRegisters(Registers regs) {
    printf("\n");
    printf("A: %u\n", regs.A);
    printf("X: %u\n", regs.X);
    printf("Y: %u\n", regs.Y);
    printf("SP: %u\n", regs.SP);
    printf("SR: %u\n\n", regs.SR.byte);
    
    printf("C: %u\n", regs.SR.C);
    printf("Z: %u\n", regs.SR.Z);
    printf("I: %u\n", regs.SR.I);
    printf("D: %u\n", regs.SR.D);
    printf("B: %u\n", regs.SR.B);
    printf("R: -\n");
    printf("V: %u\n", regs.SR.V);
    printf("N: %u\n", regs.SR.N);
}