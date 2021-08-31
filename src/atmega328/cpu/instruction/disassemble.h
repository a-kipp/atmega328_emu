#pragma once

#include "decode.h"


#define INFO_LENGTH 80

typedef struct InstructionInfo {
    uint8_t length;
    char info[INFO_LENGTH];
} InstructionInfo;




InstructionInfo unknown_disassemble(uint16_t opCode) {
    char *instructionName = "unknown";
    
    InstructionInfo instruction = {0};

    snprintf(instruction.info, INFO_LENGTH, "%s", instructionName);

    instruction.length =  1;
    
    return instruction;
}


InstructionInfo adc_disassemble(uint16_t opCode) {
    char *instructionName = "adc";
    uint16_t rd_addr = dec_extractBits0000000111110000(opCode);
    uint16_t rr_addr = dec_extractBits0000001000001111(opCode);

    InstructionInfo instruction = {0};

    snprintf(instruction.info, INFO_LENGTH, "%s %s %s", instructionName, deb_getName(rd_addr), deb_getName(rr_addr));

    instruction.length =  1;
    
    return instruction;
}


InstructionInfo add_disassemble(uint16_t opCode) {
    char *instructionName = "add";
    uint16_t rd_addr = dec_extractBits0000000111110000(opCode);
    uint16_t rr_addr = dec_extractBits0000001000001111(opCode);

    InstructionInfo instruction = {0};
    
    snprintf(instruction.info, INFO_LENGTH, "%s %s %s", instructionName, deb_getName(rd_addr), deb_getName(rr_addr));

    instruction.length =  1;
    
    return instruction;
}


InstructionInfo cbi_disassemble(uint16_t opCode) {
    char *instructionName = "cbi";
    uint16_t ioa_addr = dec_extractBits0000000001111000(opCode) + 0x20;
    uint16_t bitNum = dec_extractBits0000000000000111(opCode);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "%s %s %1X", instructionName, deb_getName(ioa_addr), bitNum);

    instruction.length =  1;
    
    return instruction;
}    


InstructionInfo eor_disassemble(uint16_t opCode) {
    char *instructionName = "eor";
    uint16_t rd_addr = dec_extractBits0000000111110000(opCode);
    uint16_t rr_addr = dec_extractBits0000001000001111(opCode);

    InstructionInfo instruction = {0};
 
    if (rd_addr == rr_addr) {
        snprintf(instruction.info, INFO_LENGTH, "%s %s", "clr", deb_getName(rd_addr));
    } else {
        snprintf(instruction.info, INFO_LENGTH, "%s %s %s", instructionName, deb_getName(rd_addr), deb_getName(rr_addr));
    }

    instruction.length =  1;
    
    return instruction;
}


InstructionInfo in_disassemble(uint16_t opCode) {
    char *instructionName = "in";
    uint16_t ioa_addr = dec_extractBits0000011000001111(opCode) + 0x20;
    uint16_t rr_addr =  dec_extractBits0000000111110000(opCode);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "%s %s %s", instructionName, deb_getName(rr_addr), deb_getName(ioa_addr));

    instruction.length =  1;
    
    return instruction;
}    


InstructionInfo ldi_disassemble(uint16_t opCode) {
    uint16_t rd_addr =  dec_extractBits0000000011110000(opCode) + 16;
    uint8_t constData = dec_extractBits0000111100001111(opCode);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "ldi \"%s\" (%02X)", deb_getName(rd_addr), constData);

    instruction.length =  1;
    
    return instruction;
}


InstructionInfo nop_disassemble(uint16_t opCode) {
    char *instructionName = "nop";

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "%s", instructionName);

    instruction.length =  1;
    
    return instruction;
}


InstructionInfo out_disassemble(uint16_t opCode) {
    char *instructionName = "out";
    uint16_t ioa_addr = dec_extractBits0000011000001111(opCode) + 0x20;
    uint16_t rr_addr =  dec_extractBits0000000111110000(opCode);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "%s %s %s", instructionName, deb_getName(ioa_addr), deb_getName(rr_addr));

    instruction.length =  1;
    
    return instruction;
}


InstructionInfo sbiw_disassemble(uint16_t opCode) {
    char *instructionName = "sbiw";
    uint16_t rd_addr =   dec_extractBits0000000000110000(opCode) * 2 + R24;
    uint16_t constData = dec_extractBits0000000011001111(opCode);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "%s %s %02X", instructionName, deb_getName(rd_addr), constData);

    instruction.length =  1;
    
    return instruction;
}


InstructionInfo brne_disassemble(uint16_t opCode) {
    char *instructionName = "brne";
    int16_t constData = (int16_t)dec_extractBits0000001111111000(opCode);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "%s %02X", instructionName, constData);
    
    instruction.length =  1;
    
    return instruction;
}


InstructionInfo dec_disassemble(uint16_t opCode) {
    char *instructionName = "dec";
    uint16_t rd_addr = dec_extractBits0000000111110000(opCode);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "%s %s", instructionName, deb_getName(rd_addr));

    instruction.length =  1;
    
    return instruction;
}


InstructionInfo rjmp_disassemble(uint16_t opCode) {
    int16_t constAddress = (int16_t)dec_extractBits0000111111111111(opCode);
    uint16_t jumpDest_addr = (mem_programCounter + constAddress - 0xfff) % (DATA_MEMORY_END + 1);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "rjmp %04X", jumpDest_addr);

    instruction.length =  1;

    return instruction;
}


InstructionInfo sts_disassemble(uint16_t opCode) {
    uint16_t instructionFirst = mem_programMemoryFetchInstruction(mem_programCounter);
    uint16_t rr_addr = dec_extractBits0000000111110000(instructionFirst);
    uint16_t instructionSecond = mem_programMemoryFetchInstruction(mem_programCounter + 1);
    uint16_t mem_addr = instructionSecond;
    uint8_t rrContent = mem_dataMemoryRead8bit(rr_addr);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "sts %04X \"%s\"(%02X)", mem_addr, deb_getName(rr_addr), rrContent);
    
    instruction.length =  2;

    return instruction;
}
