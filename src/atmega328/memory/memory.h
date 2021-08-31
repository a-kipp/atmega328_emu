#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "map.h"
#include "loader.h"
#include "virtual_space.h"
#include "register_access.h"
#include "../global_variables.h"
#include "../utility_functions.h"
#include "../out.h"


// Public
// ____________________________________________________________________________________________________________________

uint16_t mem_programCounter = 0;


void mem_loadProgram(char* filePath) {
    loa_loadHexFile(vir_programMemory_ptr, filePath, PROGRAM_MEMORY_END);
}


uint8_t mem_dataMemoryRead8bit(uint16_t address) {
    if (address <= 0x00FF) {
        return reg_read8BitFromRegister(address);
    } else {
        return *(uint8_t*)(vir_dataMemory_ptr + address);
    }
}


uint8_t mem_eepromMemoryRead8bit(uint16_t address) {
    return *(uint8_t*)(vir_eepromMemory_ptr + address);
}


uint16_t mem_programMemoryFetchInstruction(uint16_t address) {
    if (address > PROGRAM_MEMORY_END) {
        fprintf(stderr, "end of Program Memory reached\n");
    }
    uint16_t instruction = *(uint16_t*)(vir_programMemory_ptr + address);
    return uti_byteswap16bit(instruction);
}


uint16_t mem_dataMemoryRead16bit(uint16_t address) {
    uint16_t value = *(uint16_t*)(vir_dataMemory_ptr + address);
    return value;
}


// this function shall only be used by the CPU to write to memory
void mem_dataMemoryWrite8bitCpu(uint16_t address, uint8_t value) {
    if (address <= 0x00FF) {
        reg_write8BitToRegister(address, value);
    } else if (address > DATA_MEMORY_END) {
        fprintf(stderr, "invalid address in datamemory called\n");
    } else {
        *(uint8_t*)(vir_dataMemory_ptr + address) = value;
    }
}


// this function provides restricted acces to the memory from external
void mem_externalSourceWrite8bit(uint16_t address, uint8_t value) {
    switch (address) {
        case UDR0: *(uint8_t*)(vir_dataMemory_ptr + address) = value; break;
        case PORTB: *(uint8_t*)(vir_dataMemory_ptr + address) = value; break;
        case PORTC: *(uint8_t*)(vir_dataMemory_ptr + address) = value; break;
        case PORTD: *(uint8_t*)(vir_dataMemory_ptr + address) = value; break;
        default: fprintf(stderr, "no external acces to memory address: %04X\n", address); break;
    }
}


void mem_eepromMemoryWrite8bit(uint16_t address, uint8_t value) {
    address %= EEPROM_END;
    *(uint8_t*)(vir_eepromMemory_ptr + address) = value;
}


void mem_dataMemoryWrite16bit(uint16_t address, uint16_t value) {
    *(uint16_t*)(vir_dataMemory_ptr + address) = value;
}


