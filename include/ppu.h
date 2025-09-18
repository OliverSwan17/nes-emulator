#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "intdefs.h"

typedef struct PPURegs
{
    u16 v;
    u16 t;
    u16 x;
    u16 w;
} PPURegs;

void readPatternTable();