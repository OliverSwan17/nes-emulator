#pragma once
#include "defs.h"

typedef struct Registers {
    u8 A;  // Accumulator
    u8 X;  // Index
    u8 Y;  // Index
    u16 PC; // Program counter
    u8 SP; // Stack pointer
    
    // Status Register (SR)
    union {
        u8 SR; // The whole Status Register byte
        struct {
            u8 Carry : 1;            // Carry flag (bit 0)
            u8 Zero : 1;             // Zero flag (bit 1)
            u8 InterruptDisable : 1; // Interrupt Disable (bit 2)
            u8 Decimal : 1;          // Decimal Mode flag (bit 3)
            u8 BreakCommand : 1;     // Break Command (bit 4)
            u8 Reserved : 1;        // Future use (bit 5)
            u8 Overflow : 1;         // Overflow flag (bit 6)
            u8 Negative : 1;         // Negative flag (bit 7)
        };
    } SR;
} Registers;
