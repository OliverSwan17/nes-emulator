#include "6502.h"
#include "rom.h"
#include "draw.h"
#include "ppu.h"

extern Registers regs;
extern Memory memory;
extern u8 *rom;
extern size_t romSize;
extern ROM_Header romHeader;

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    
    load_rom("roms/Super_mario_brothers.nes", &rom, &romHeader);
    readPatternTable();
    //load_rom("roms/6502_test.bin", &rom, &romHeader);

    printf("Magic: %.4s, PRG: %dKB, CHR: %dKB, Mapper: %d, Flags: 6=0x%02X 7=0x%02X 8=0x%02X 9=0x%02X 10=0x%02X\n", 
       romHeader.magic, romHeader.prgRomSize * 16, romHeader.chrRomSize * 8, 
       ((romHeader.flags7 & 0xF0) | (romHeader.flags6 >> 4)), 
       romHeader.flags6, romHeader.flags7, romHeader.flags8, romHeader.flags9, romHeader.flags10);
    printf("ROM Size: %llu Bytes\n", romSize);

    initDraw();
    powerUp(); // 6502
    cleanupDraw();

    return 0;
}