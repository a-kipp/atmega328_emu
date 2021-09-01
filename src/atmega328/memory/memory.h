#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "map.h"
#include "loader.h"
#include "array.h"
#include "register_access.h"
#include "../global_variables.h"
#include "../utility_functions.h"
#include "../out.h"

uint16_t mem_programCounter = 0;






// Public Access Data Memory
// ____________________________________________________________________________________________________________________


// this function shall only be used by the CPU to write to memory
void mem_dataWrite8bitFromCPU(uint16_t address, uint8_t value) {
    if (address <= 0x00FF) {
        reg_write8BitToRegister(address, value);
    } else if (address > DATA_MEMORY_END) {
        fprintf(stderr, "invalid address in datamemory called\n");
    } else {
        arr_dataMemory[address] = value;
    }
}


// this function provides restricted acces to the memory from external
void mem_dataWrite8bitFromExtern(uint16_t address, uint8_t value) {
    switch (address) {
        case UDR0: arr_dataMemory[address] = value; break;
        case PORTB: arr_dataMemory[address] = value; break;
        case PORTC: arr_dataMemory[address] = value; break;
        case PORTD: arr_dataMemory[address] = value; break;
        default: fprintf(stderr, "no external acces to memory address: %04X\n", address); break;
    }
}


// unrestricted write acces to memory
void mem_dataWrite8bitUnconditional(uint16_t address, uint8_t value) {
    arr_dataMemory[address] = value;
}


uint8_t mem_dataRead8bitFromCPU(uint16_t address) {
    if (address <= 0x00FF) {
        return reg_read8BitFromRegister(address);
    } else {
        return arr_dataMemory[address];
    }
}


uint8_t mem_dataRead8bitUnconditional(uint16_t address) {
    return arr_dataMemory[address];
}


uint16_t mem_dataMemoryRead16bit(uint16_t address) {
    uint16_t value = *(uint16_t*)(vir_dataMemory_ptr + address);
    return value;
}


void mem_dataMemoryWrite16bit(uint16_t address, uint16_t value) {
    *(uint16_t*)(vir_dataMemory_ptr + address) = value;
}








// Public Access Program Memory
// ____________________________________________________________________________________________________________________


void mem_loadProgram(char* filePath) {
    loa_loadHexFile(vir_programMemory_ptr, filePath, PROGRAM_MEMORY_END);
}


uint16_t mem_programFetchInstruction(uint16_t address) {
    if (address > PROGRAM_MEMORY_END) {
        fprintf(stderr, "end of Program Memory reached\n");
    }
    uint16_t opCode = arr_programMemory[address];
    return uti_byteswap16bit(opCode);
}







// Public Access Eeprom Memory
// ____________________________________________________________________________________________________________________


void mem_eepromWrite8bitUnconditional(uint16_t address, uint8_t value) {
    arr_eepromMemory[address] = value;
}


uint8_t mem_eepromRead8bitUnconditional(uint16_t address) {
    return arr_eepromMemory[address];
}


