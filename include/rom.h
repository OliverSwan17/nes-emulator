#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#pragma pack(push, 1)    // Ensure no padding between fields
typedef struct ROM_Header {
    uint8_t magic[4];    // "NES\x1A"
    uint8_t prgRomSize;  // PRG ROM size in 16KB units
    uint8_t chrRomSize;  // CHR ROM size in 8KB units
    uint8_t flags6;      // Mapper, mirroring, battery, trainer
    uint8_t flags7;      // Mapper, VS/Playchoice, NES 2.0
    uint8_t flags8;      // PRG-RAM size
    uint8_t flags9;      // TV system
    uint8_t flags10;     // TV system, PRG-RAM presence
    uint8_t padding[5];  // Unused bytes 11-15
} ROM_Header;

#pragma pack(pop)

void load_rom(const char* filename, uint8_t** rom, ROM_Header *romHeader);