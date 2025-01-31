#include "main.h"

int main() {
    Registers regs;

    // Dummy values for testing
    regs.A = 17;
    regs.SR.Carry = 1;
    displayRegisters(regs);

    return 0;
}