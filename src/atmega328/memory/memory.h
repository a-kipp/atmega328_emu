#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "map.h"
#include "loader.h"
#include "../utils.h"
#include "../peripherals.h"
//#include "interrupts.h"
#include "arrays.h"
#include "register.h"
#include "../interrupts.h"




// Access Data Memory
// ____________________________________________________________________________________________________________________


// this function shall only be used by the CPU to write to memory
void acc_dataWrite8bit(uint16_t address, uint8_t value) {
    if (address <= 0x00FF) {
        reg_write8BitToRegister(address, value);
    } else if (address > DATA_MEMORY_END) {
        fprintf(stderr, "invalid address in datamemory called\n");
    } else {
        mem_dataMemory[address] = value;
    }
}




// unrestricted write acces to memory
//void mem_dataWrite8bitUnconditional(uint16_t address, uint8_t value) {
    //if (address == SREG) {
    //    fprintf(stderr, "direct write to sreg", address);
    //    exit(-1);
    //}
//    mem_dataMemory[address] = value;
//}


uint8_t acc_dataRead8bit(uint16_t address) {
    if (address <= 0x00FF) {
        return reg_read8BitFromRegister(address);
    } else {
        return mem_dataMemory[address];
    }
}


uint16_t acc_dataRead16bit(uint16_t address) {
    uint16_t returnValue = acc_dataRead8bit(address);
    returnValue |= ((uint16_t)acc_dataRead8bit(address + 1)) << 8;
    return returnValue;
}


void acc_dataWrite16bit(uint16_t address, uint16_t value) {
    acc_dataWrite8bit(address, (uint8_t)value);
    acc_dataWrite8bit(address + 1, (uint8_t)(value >> 8));
}








// Access Program Memory
// ____________________________________________________________________________________________________________________


void mem_loadProgram(char* filePath) {
    loa_loadHexFile(mem_programMemory, filePath, PROGRAM_MEMORY_END);
}


uint16_t mem_fetchInstruction(uint16_t address) {
    if (cpu_programCounter > PROGRAM_MEMORY_END) {
        fprintf(stderr, "end of Program Memory reached\n");
    }
    uint16_t opCode = mem_programMemory[address];
    return uti_byteswap16bit(opCode);
}







// Access Eeprom Memory
// ____________________________________________________________________________________________________________________


void mem_eepromWrite8bit(uint16_t address, uint8_t value) {
    mem_eepromMemory[address] = value;
}


uint8_t mem_eepromRead8bit(uint16_t address) {
    return mem_eepromMemory[address];
}








