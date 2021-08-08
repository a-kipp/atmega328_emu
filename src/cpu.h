#include <stdint.h>
#include "utility_functions.h"
#include "instruction_table.h"


void cpu_execute_single_instruction() {

void cpu_execute_single_instruction() {
    uint16_t instruction = memory_program_read_16bit(global_program_counter);
    (*instruction_table[instruction])(instruction);
} 