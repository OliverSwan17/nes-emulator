#include "utils.h"

void displayRegisters(Registers regs) {
    printf("A: %u\n", regs.A);
    printf("B: %u\n", regs.SR.Carry);
}