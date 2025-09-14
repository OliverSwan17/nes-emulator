#include "ppu.h"
#include "rom.h"

extern u8 *rom;
extern ROM_Header romHeader;
u8 patternTable[0x1000];
u8 pixels[256][8][8]; // Has value 0, 1, 2, or 3

void readPatternTable() {
    //memcpy(patternTable, rom + (romHeader.prgRomSize * 16 * 1024), 0x1000);
    memcpy(patternTable, rom + (romHeader.prgRomSize * 16 * 1024) + 0x1000, 0x1000);
    for (int tile = 0; tile < 256; tile++) {
        for(int y = 0; y < 8; y++) {
            for(int x = 0; x < 8; x++) {
                int bitplane1value = (1 << x) & patternTable[tile * 16 + y]; 
                int bitplane2Value = (1 << x) & patternTable[tile * 16 + y + 8]; 
                
                int pixelValue;
                if (!(bitplane1value || bitplane2Value))
                    pixelValue = 0;
                else if(bitplane1value && !bitplane2Value)
                    pixelValue = 1;
                else if(!bitplane1value && bitplane2Value)
                    pixelValue = 2;
                else if(bitplane1value && bitplane2Value)
                    pixelValue = 3;
                
                pixels[tile][y][7-x] = pixelValue;
            }
        }
    }

    for (int tile = 0; tile < 256; tile++) {
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                printf("%x", pixels[tile][y][x]);
            }
            printf("\n");
        }
        printf("\n");
    }
}