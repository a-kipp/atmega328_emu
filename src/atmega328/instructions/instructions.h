#pragma once

#include "jump_table_disassemble.h"
#include "disassemble.h"
#include "../memory/map.h"
;


// Public
// ____________________________________________________________________________________________________________________


InstructionInfo ins_disassembleInstruction(uint16_t programCounter) {
    uint16_t opCode = mem_fetchInstruction(programCounter);
    return jtd_disassembleTable[opCode](opCode, programCounter);
}

// TODO make it disassemble more than 100 instructions
void ins_disassembleProgram() {
    uint16_t programCounter;
    while (programCounter < 100) {
        InstructionInfo info = ins_disassembleInstruction(programCounter);
        printf("%04X ",  programCounter);
        printf("%04X ", uti_byteswap16bit(mem_fetchInstruction(programCounter)));
        printf(" %s",  info.info);
        printf("     %s\n",  info.comment);
        programCounter += info.length;
    }
}