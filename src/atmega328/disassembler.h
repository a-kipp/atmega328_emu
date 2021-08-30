#pragma once

#include "cpu/instruction_implementations.h"
#include "memory/memory_map.h"
#include "cpu/jump_table_disassemble.h"
;

static void _disassemble() {
    for (int i = 0; i < 20; i++) {
        uint16_t instruction = mem_programMemoryFetchInstruction(ins_lineCounter);
        jump_table_disassemble[instruction]();
    }
}


// Public
// ____________________________________________________________________________________________________________________


char *dis_getDisassembledInstruction(uint16_t address) {
    if (ins_disassembledInstructions[0][0] == 0) {
        _disassemble();
    }
    if (ins_disassembledInstructions[0][0] == 0) {
        fprintf(stderr, "disassembly is empty\n");
    }
    return ins_disassembledInstructions[address];
}


void dis_printDisassembledInstructions() {
    
    if (ins_disassembledInstructions[0][0] == 0) {
        _disassemble();
    }
    for (int i = 0; i <=  20; i++) {
        printf("%s\n", ins_disassembledInstructions[i]);
    }
}




