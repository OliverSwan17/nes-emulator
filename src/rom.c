#include "rom.h"

int load_rom(const char* filename, uint8_t** rom, ROM_Header *romHeader) {
    FILE* file = fopen(filename, "rb");
    if (!file)
        return -1;

    // Read header
    fread(romHeader, sizeof(ROM_Header), 1, file);
    
    // Get ROM data size
    fseek(file, 0, SEEK_END);
    size_t romSize = ftell(file) - 16;
    fseek(file, 16, SEEK_SET);

    // Read data
    *rom = malloc(romSize);
    fread(*rom, 1, romSize, file);
    
    fclose(file);
    return romSize;
}