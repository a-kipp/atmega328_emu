#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "memory_map.h"
#include "loader.h"
#include "../global_variables.h"
#include "../utility_functions.h"
#include "../out.h"

static uint16_t *_programMemory_ptr;
static uint8_t *_dataMemory_ptr;
static uint8_t *_eepromMemory_ptr;


// Public
// ____________________________________________________________________________________________________________________

uint16_t mem_programCounter = 0;


// for perfomance reason flags are stored separatly, reading from SREG register will be intercepted.
bool mem_sregCarryFlagC = 0;
bool mem_sregZeroFlagZ = 0;
bool mem_sregNegativeFlagN = 0;
bool mem_sregTwoComplementsOverflowFlagV = 0;
bool mem_sregSignBitS = 0;
bool mem_sregHalfCarryFlagH = 0;
bool mem_sregBitCopyStorageT = 0;
bool mem_sregGlobalInterruptEnableI = 0;

static void _writeToSreg(uint8_t value) {
    bool sregCarryFlagC = value ^ 0b00000001;
    bool sregZeroFlagZ = value ^ 0b00000010;
    bool sregNegativeFlagN = value ^ 0b00000100;
    bool sregTwoComplementsOverflowFlagV = value ^ 0b00001000;
    bool sregSignBitS = value ^ 0b00010000;
    bool sregHalfCarryFlagH = value ^ 0b00100000;
    bool sregBitCopyStorageT = value ^ 0b01000000;
    bool sregGlobalInterruptEnableI = value ^ 0b10000000;
}

static uint8_t _readFromSreg() {
    uint8_t returnVal = mem_sregCarryFlagC;
    returnVal | mem_sregCarryFlagC;
    returnVal | mem_sregZeroFlagZ;
    returnVal | mem_sregNegativeFlagN;
    returnVal | mem_sregTwoComplementsOverflowFlagV;
    returnVal | mem_sregSignBitS;
    returnVal | mem_sregHalfCarryFlagH;
    returnVal | mem_sregBitCopyStorageT;
    returnVal | mem_sregGlobalInterruptEnableI;
    return returnVal;
}



void mem_init() {
    _programMemory_ptr = calloc(PROGRAM_MEMORY_END + 1, sizeof(uint8_t));
    _dataMemory_ptr = calloc(DATA_MEMORY_END + 1, sizeof(uint8_t));
    _eepromMemory_ptr = calloc(EEPROM_END + 1, sizeof(uint8_t));
}


void mem_loadProgram(char* filePath) {
    loa_loadHexFile(_programMemory_ptr, filePath, PROGRAM_MEMORY_END);
}


uint8_t mem_dataMemoryRead8bit(uint16_t address) {
    if (address = SREG) {
        return _readFromSreg();
    }
    uint val = *(uint8_t*)(_dataMemory_ptr + address);
    return *(uint8_t*)(_dataMemory_ptr + address);
}


uint8_t mem_eepromMemoryRead8bit(uint16_t address) {
    return *(uint8_t*)(_eepromMemory_ptr + address);
}


uint16_t mem_programMemoryFetchInstruction(uint16_t address) {
    uint16_t instruction = *(uint16_t*)(_programMemory_ptr + address);
    return uti_byteswap16bit(instruction);
}


uint16_t mem_dataMemoryRead16bit(uint16_t address) {
    uint16_t value = *(uint16_t*)(_dataMemory_ptr + address);
    return value;
}


// this function shall only be used by the CPU to write to memory
void mem_dataMemoryWrite8bitCpu(uint16_t address, uint8_t value) {
    if (address <= GENERAL_PURPOSE_REGISTERS_END) {
        if (address = SREG) {
            _writeToSreg(value);
            return;
        }
    }    
    else if (address <= IO_REGISTERS_END) {
        *(uint8_t*)(_dataMemory_ptr + address) = value;
    }
    else if (address <= EXTERN_IO_REGISTERS_END) {
        switch (address) {
            case UDR0: out_serialOut(value); break;
            default: *(uint8_t*)(_dataMemory_ptr + address) = value; break;
        }
    }
    else if (address <= INTERNAL_SRAM_END) {
        *(uint8_t*)(_dataMemory_ptr + address) = value;
    }
    else {
        fprintf(stderr, "data memory address out of range max: %03X\n", DATA_MEMORY_END);
    }
}


// this function provides restricted acces to the memory from external
void mem_externalSourceWrite8bit(uint16_t address, uint8_t value) {
    switch (address) {
        case UDR0: *(uint8_t*)(_dataMemory_ptr + address) = value; break;
        case PORTB: *(uint8_t*)(_dataMemory_ptr + address) = value; break;
        case PORTC: *(uint8_t*)(_dataMemory_ptr + address) = value; break;
        case PORTD: *(uint8_t*)(_dataMemory_ptr + address) = value; break;
        default: fprintf(stderr, "no external acces to memory address: %04X\n", address); break;
    }
}


void mem_eepromMemoryWrite8bit(uint16_t address, uint8_t value) {
    *(uint8_t*)(_eepromMemory_ptr + address) = value;
}


void mem_dataMemoryWrite16bit(uint16_t address, uint16_t value) {
    *(uint16_t*)(_dataMemory_ptr + address) = value;
}


