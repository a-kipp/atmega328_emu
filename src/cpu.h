#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "utility_functions.h"
#include "memory_map.h"
#include "global_variables.h"
#include "instruction_table.h"


void execute_single_instruction() {
    uint16_t instruction = *(program_memory_ptr + program_counter);
    (*instruction_table[instruction])(instruction);
} 