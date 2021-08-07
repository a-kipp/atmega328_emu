#include <stdint.h>
#include "instruction_implementations.h"

void (*instruction_table[])(uint16_t) = {
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    nop,
    //65536 inctruction will go here...
}