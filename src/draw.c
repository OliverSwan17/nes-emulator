#include "draw.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern Registers regs;
extern Memory memory;
extern u8 pixels[2][256][8][8];

// Colors
#undef COLOR_WHITE
#undef COLOR_BLACK
#define SDL_COLOR_WHITE {255, 255, 255, 255}
#define SDL_COLOR_BLACK {0, 0, 0, 255}
#define SDL_COLOR_HIGHLIGHT {255, 255, 0, 255}

// SDL2 context
typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    int char_width;
    int char_height;
} DrawContext;

static DrawContext g_drawCtx = {0};
static void drawString(int col, int row, const char* text, int highlight);
static void clearScreen(void);
static void drawZeroPage(void);
static void drawStack(u8 spOffset);
static void drawRegisters(void);
static void drawStatusBits(void);
static void drawInstructionToExecute(Instruction instruction);
static void drawProgram(u8 pcOffset);
static void drawPatternTables(void);

int initDraw(void) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    g_drawCtx.window = SDL_CreateWindow("6502 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_SHOWN);
    g_drawCtx.renderer = SDL_CreateRenderer(g_drawCtx.window, -1, SDL_RENDERER_ACCELERATED);
    g_drawCtx.font = TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 16);
    TTF_SizeText(g_drawCtx.font, "A", &g_drawCtx.char_width, &g_drawCtx.char_height);
    return 0;
}

void cleanupDraw(void) {
    if (g_drawCtx.font) TTF_CloseFont(g_drawCtx.font);
    if (g_drawCtx.renderer) SDL_DestroyRenderer(g_drawCtx.renderer);
    if (g_drawCtx.window) SDL_DestroyWindow(g_drawCtx.window);
    TTF_Quit();
    SDL_Quit();
}

static void drawString(int col, int row, const char* text, int highlight) {
    SDL_Color color = highlight ? (SDL_Color)SDL_COLOR_HIGHLIGHT : (SDL_Color)SDL_COLOR_WHITE;
    SDL_Surface* surface = TTF_RenderText_Solid(g_drawCtx.font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(g_drawCtx.renderer, surface);
    SDL_Rect destRect = {col * g_drawCtx.char_width, row * g_drawCtx.char_height, surface->w, surface->h};

    SDL_RenderCopy(g_drawCtx.renderer, texture, NULL, &destRect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

static void clearScreen(void) {
    SDL_SetRenderDrawColor(g_drawCtx.renderer, 0, 0, 0, 255);
    SDL_RenderClear(g_drawCtx.renderer);
}

static void presentScreen(void) {
    SDL_RenderPresent(g_drawCtx.renderer);
}

void draw(Instruction instruction) {
    clearScreen();
    drawZeroPage();
    drawStack(regs.SP);
    drawProgram(regs.PC - PROGRAM_OFFSET);
    drawRegisters();
    drawStatusBits();
    drawInstructionToExecute(instruction);
    drawPatternTables();
    presentScreen();
    
    SDL_Event e;
    int waiting = 1;
    while (waiting) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                cleanupDraw();
                exit(0);
            }
            if (e.type == SDL_KEYDOWN) {
                waiting = 0;
            }
        }
        SDL_Delay(16);
    }
}

static void drawPatternTables(void) {
    SDL_Color colors[4] = {
        {0, 0, 0, 255},       // 0: Black
        {85, 85, 85, 255},    // 1: Dark Gray
        {170, 170, 170, 255}, // 2: Light Gray
        {255, 255, 255, 255}  // 3: White
    };
    
    int start_x = 50 * g_drawCtx.char_width;
    int start_y = 15 * g_drawCtx.char_height;
    int pixel_size = 2;
    int table_spacing = 272;
    
    for (int table = 0; table < 2; table++) {
        int table_start_x = start_x + (table * table_spacing);
        
        for (int tile = 0; tile < 256; tile++) {
            int tile_x = tile % 16;
            int tile_y = tile / 16;
            
            int screen_tile_x = table_start_x + (tile_x * 8 * pixel_size);
            int screen_tile_y = start_y + (tile_y * 8 * pixel_size);
            
            for (int y = 0; y < 8; y++) {
                for (int x = 0; x < 8; x++) {
                    u8 pixel_value = pixels[table][tile][y][x];
                    SDL_Color color = colors[pixel_value];
                    SDL_SetRenderDrawColor(g_drawCtx.renderer, color.r, color.g, color.b, color.a);
                    SDL_Rect pixel_rect = {
                        screen_tile_x + (x * pixel_size),
                        screen_tile_y + (y * pixel_size),
                        pixel_size,
                        pixel_size
                    };
                    SDL_RenderFillRect(g_drawCtx.renderer, &pixel_rect);
                }
            }
        }
        
        SDL_SetRenderDrawColor(g_drawCtx.renderer, 128, 128, 128, 255);
        SDL_Rect border = {
            table_start_x - 1,
            start_y - 1,
            (16 * 8 * pixel_size) + 2,
            (16 * 8 * pixel_size) + 2
        };
        SDL_RenderDrawRect(g_drawCtx.renderer, &border);
    }
}

void drawZeroPage(void) {
    char buffer[4];
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            snprintf(buffer, sizeof(buffer), "%02X ", memory.ram[i * 16 + j]);
            drawString(j * 3, i, buffer, 0);
        }
    }
}

void drawStack(u8 spOffset) {
    char buffer[4];
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            int highlight = (j + i * 16 == spOffset) ? 1 : 0;
            snprintf(buffer, sizeof(buffer), "%02X ", memory.ram[0x100 + i * 16 + j]);
            drawString(j * 3, 17 + i, buffer, highlight);
        }
    }
}

void drawRegisters(void) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), " A: 0x%02X", regs.A);
    drawString(50, 0, buffer, 0);
    snprintf(buffer, sizeof(buffer), " X: 0x%02X", regs.X);
    drawString(50, 1, buffer, 0);
    snprintf(buffer, sizeof(buffer), " Y: 0x%02X", regs.Y);
    drawString(50, 2, buffer, 0);
    snprintf(buffer, sizeof(buffer), "SP: 0x%02X", regs.SP);
    drawString(50, 3, buffer, 0);
    snprintf(buffer, sizeof(buffer), "PC: 0x%04X", regs.PC);
    drawString(50, 4, buffer, 0);
    snprintf(buffer, sizeof(buffer), "SR: 0x%02X", regs.SR.byte);
    drawString(50, 5, buffer, 0);
}

void drawStatusBits(void) {
    char buffer[32];
    drawString(62, 0, "C Z I D B R V N", 0);
    snprintf(buffer, sizeof(buffer), "%u", regs.SR.C);
    drawString(62, 1, buffer, 0);
    snprintf(buffer, sizeof(buffer), "%u", regs.SR.Z);
    drawString(64, 1, buffer, 0);
    snprintf(buffer, sizeof(buffer), "%u", regs.SR.I);
    drawString(66, 1, buffer, 0);
    snprintf(buffer, sizeof(buffer), "%u", regs.SR.D);
    drawString(68, 1, buffer, 0);
    snprintf(buffer, sizeof(buffer), "%u", regs.SR.B);
    drawString(70, 1, buffer, 0);
    drawString(72, 1, "-", 0);
    snprintf(buffer, sizeof(buffer), "%u", regs.SR.V);
    drawString(74, 1, buffer, 0);
    snprintf(buffer, sizeof(buffer), "%u", regs.SR.N);
    drawString(76, 1, buffer, 0);
}

void drawInstructionToExecute(Instruction instruction) {
    char mode[20];
    char buffer[64];
    
    switch (instruction.addressingMode) {
        case ACCUMULATOR:
            strcpy(mode, "ACCUMULATOR");
            break;
        case ABSOLUTE:
            strcpy(mode, "ABSOLUTE");
            break;
        case ABSOLUTE_X:
            strcpy(mode, "ABSOLUTE_X");
            break;
        case ABSOLUTE_Y:
            strcpy(mode, "ABSOLUTE_Y");
            break;
        case IMMEDIATE:
            strcpy(mode, "IMMEDIATE");
            break;
        case IMPLIED:
            strcpy(mode, "IMPLIED");
            break;
        case INDIRECT:
            strcpy(mode, "INDIRECT");
            break;
        case X_INDIRECT:
            strcpy(mode, "X_INDIRECT");
            break;
        case INDIRECT_Y:
            strcpy(mode, "INDIRECT_Y");
            break;
        case RELATIVE:
            strcpy(mode, "RELATIVE");
            break;
        case ZEROPAGE:
            strcpy(mode, "ZEROPAGE");
            break;
        case ZEROPAGE_X:
            strcpy(mode, "ZEROPAGE_X");
            break;
        case ZEROPAGE_Y:
            strcpy(mode, "ZEROPAGE_Y");
            break;
    }

    snprintf(buffer, sizeof(buffer), "Mnemonic: %s", instruction.mnemonic);
    drawString(50, 7, buffer, 0);
    snprintf(buffer, sizeof(buffer), "Addressing Mode: %s", mode);
    drawString(50, 8, buffer, 0);
    snprintf(buffer, sizeof(buffer), "Opcode byte: 0x%02X", instruction.opcode.byte);
    drawString(50, 9, buffer, 0);
    snprintf(buffer, sizeof(buffer), "Operand bytes: 0x%04X", instruction.operand.bytes);
    drawString(50, 10, buffer, 0);
    snprintf(buffer, sizeof(buffer), "Bytes: %u", instruction.bytes);
    drawString(50, 11, buffer, 0);
    snprintf(buffer, sizeof(buffer), "Cycles: %u", instruction.cycles);
    drawString(50, 12, buffer, 0);
}

void drawProgram(u8 pcOffset) {
    char buffer[4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 16; j++) {
            int highlight = (j + i * 16 == pcOffset) ? 1 : 0;
            snprintf(buffer, sizeof(buffer), "%02X ", memory.program[i * 16 + j]);
            drawString(j * 3, 34 + i, buffer, highlight);
        }
    }
}

// Keep the original printf functions for compatibility
void printRegisters(Registers regs) {
    printf("------------------------------------------------\n");
    printf("A: 0x%02X\n", regs.A);
    printf("X: 0x%02X\n", regs.X);
    printf("Y: 0x%02X\n", regs.Y);
    printf("SP: 0x%02X\n", regs.SP);
    printf("PC: 0x%04X\n", regs.PC);
    printf("SR: 0x%02X\n\n", regs.SR.byte);
    
    printf("C Z I D B R V N\n");

    printf("%u ", regs.SR.C);
    printf("%u ", regs.SR.Z);
    printf("%u ", regs.SR.I);
    printf("%u ", regs.SR.D);
    printf("%u ", regs.SR.B);
    printf("- ");
    printf("%u ", regs.SR.V);
    printf("%u \n", regs.SR.N);
}

void printInstruction(Instruction instruction) {
    printf("------------------------------------------------\n");
    printf("Opcode byte: 0x%02X\n", instruction.opcode.byte);
    printf("Low Nibble: 0x%X\n", instruction.opcode.lowNibble);
    printf("High Nibble: 0x%X\n\n", instruction.opcode.highNibble);

    printf("Operand bytes: 0x%04X\n", instruction.operand.bytes);
    printf("Low Byte: 0x%02X\n", instruction.operand.lowByte);
    printf("High Byte: 0x%02X\n\n", instruction.operand.highByte);

    printf("Mnemonic: %s\n", instruction.mnemonic);

    printf("Addressing Mode: ");
    switch (instruction.addressingMode) {
        case IMPLIED:
            printf("IMPLIED\n");
            break;
        case ABSOLUTE:
            printf("ABSOLUTE\n");
            break;
        default:
            printf("TO_BE_IMPLEMENTED\n");
            break;
    }

    printf("Cycles: %u\n", instruction.cycles);
    printf("Bytes: %u\n", instruction.bytes);
}
