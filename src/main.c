#include "6502.h"
#include "rom.h"

extern Registers regs;
extern Memory memory;

int main() {
    uint8_t *rom;
    ROM_Header romHeader;

    size_t romSize = load_rom("roms/Super_mario_brothers.nes", &rom, &romHeader);
    printf("Magic: %.4s, PRG: %dKB, CHR: %dKB, Mapper: %d, Flags: 6=0x%02X 7=0x%02X 8=0x%02X 9=0x%02X 10=0x%02X\n", 
       romHeader.magic, romHeader.prgRomSize * 16, romHeader.chrRomSize * 8, 
       ((romHeader.flags7 & 0xF0) | (romHeader.flags6 >> 4)), 
       romHeader.flags6, romHeader.flags7, romHeader.flags8, romHeader.flags9, romHeader.flags10);
    printf("ROM Size: %llu Bytes\n", romSize);

    // Pdcurses
    initscr();
    resize_term(40, 80);
    PDC_set_title("6502");

    // 6502
    powerUp();

    endwin();
    return 0;
}
