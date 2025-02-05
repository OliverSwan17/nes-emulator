#include "6502.h"

extern Registers regs;
extern Memory memory;

int main() {
    // Pdcurses
    initscr();
    resize_term(40, 80);
    PDC_set_title("6502");

    // 6502
    powerUp();

    endwin();
    return 0;
}
