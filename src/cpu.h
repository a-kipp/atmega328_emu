#include <stdint.h>
#include "instruction_table.h"
#include "memory.h"


void cpu_execute_single_instruction() {
    uint16_t instruction = memory_program_read_16bit(global_program_counter);
    (*instruction_table[instruction])(instruction);
} 