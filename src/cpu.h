#pragma once

#include <stdint.h>
#include <time.h>
#include "instruction_table.h"
#include "memory.h"

;
void _execute_single_instruction() {
    uint16_t instruction = mem_programMemoryFetchInstruction(g_programCounter);
    instruction_table[instruction]();
}

void cpu_run() {
    clock_t start = clock(), diff;

    for (int i = 0; i < 16000000; i++) {
        _execute_single_instruction();
    }
    diff = clock() - start;

    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Time taken %d seconds %d milliseconds\n", msec/1000, msec%1000);
}