#include "6502.h"

extern Registers regs;
extern Memory memory;

int main() {
    initscr();
    PDC_set_title("6502");
    initInstructionMetaData();
    powerUp();

    drawZeroPage();
    drawProgram(3);
    drawRegisters();
    drawStatusBits();
    Instruction instruction = identifyInstruction((u8 *)&memory + sizeof(Memory) - sizeof(memory.program));
    drawInstructionToExecute(instruction);
    refresh();
    getch();
    endwin();
    
    return 0;
}