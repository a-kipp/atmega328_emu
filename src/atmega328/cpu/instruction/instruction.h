#pragma once

#include "jump_table_disassemble.h"
#include "disassemble.h"
#include "../../memory/map.h"
;

char lookUpTableInfoString[INFO_STRING_LENGTH][0x1000] = {0};
uint8_t lookUpTableLength[0x1000] = {0};


static void _generateLookUpTable() {
    for (int i = 0; i < 0x1000; i++) {
        int opCode = i;
        (uint16_t)opCode;
        InstructionInfo info;
        info = jtd_disassembleTable[opCode](opCode);

        strncpy(lookUpTableInfoString[opCode], info.infoString, INFO_STRING_LENGTH);
        lookUpTableLength[opCode] = jtd_disassembleTable[opCode](opCode).length;
    }
}


// Public
// ____________________________________________________________________________________________________________________

uint8_t ins_getInstructionLength(uint16_t opCode) {
    if (lookUpTableLength[0] == 0) {
        _generateLookUpTable();
    } else {
        return lookUpTableLength[opCode];
    }
}
