#pragma once

#include <stdint.h>
#include "instruction_table.h"
#include "memory.h"
;
void cpu_execute_single_instruction() {
    uint16_t instruction = mem_programMemoryFetchInstruction(g_programCounter);
    instruction_table[instruction]();
} 