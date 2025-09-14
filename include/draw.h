#pragma once
#include "6502.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Initialize/cleanup SDL2 drawing system
int initDraw(void);
void cleanupDraw(void);

// Main drawing function
void draw(Instruction instruction);

// Unused (kept for compatibility)
void printRegisters(Registers regs);
void printInstruction(Instruction instruction);