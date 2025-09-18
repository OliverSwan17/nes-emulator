#include "ppu.h"
#include "rom.h"

extern u8 *rom;
extern ROM_Header romHeader;

u8 patternTable[0x2000];
u8 pixels[2][256][8][8]; // Has value 0, 1, 2, or 3
PPURegs ppuRegs;
u8 vRam[0x800];
u8 paletteRam[0x20];

void readPatternTable() {
    memcpy(patternTable, rom + (romHeader.prgRomSize * 16 * 1024), 0x1000);
    memcpy(patternTable + 0x1000, rom + (romHeader.prgRomSize * 16 * 1024) + 0x1000, 0x1000);
    for (int tile = 0; tile < 512; tile++) {
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
                
                pixels[(tile < 256) ? 0 : 1][tile % 256][y][7-x] = pixelValue;
            }
        }
    }
}
