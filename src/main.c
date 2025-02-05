#include "6502.h"

int main() {
    initscr();
    PDC_set_title("6502");
    initInstructionMetaData();
    powerUp();

    drawZeroPage();
    drawProgram();
    drawRegisters();
    refresh();
    getch();
    endwin();
    
    return 0;
}