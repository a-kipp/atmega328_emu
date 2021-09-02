#pragma once

#include "jump_table_disassemble.h"
#include "disassemble.h"
#include "../memory/map.h"
;


// Public
// ____________________________________________________________________________________________________________________


InstructionInfo ins_info(uint16_t opCode) {
    return jtd_disassembleTable[opCode](opCode);
}

