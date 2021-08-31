#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "map.h"
#include "loader.h"
#include "../global_variables.h"
#include "../utility_functions.h"
#include "../out.h"

uint16_t _programMemory[PROGRAM_MEMORY_END] = {0};
uint8_t _dataMemory[DATA_MEMORY_END] = {0};
uint8_t _eepromMemory[EEPROM_END] = {0};
uint16_t *_programMemory_ptr = _programMemory;
uint8_t *_dataMemory_ptr = _dataMemory;
uint8_t *_eepromMemory_ptr = _eepromMemory;


// Public
// ____________________________________________________________________________________________________________________

uint16_t mem_programCounter = 0;


// for easier access flags are stored separatly, readings and writes from SREG register will be intercepted.
bool mem_sregCarryFlagC = 0;
bool mem_sregZeroFlagZ = 0;
bool mem_sregNegativeFlagN = 0;
bool mem_sregTwoComplementsOverflowFlagV = 0;
bool mem_sregSignBitS = 0;
bool mem_sregHalfCarryFlagH = 0;
bool mem_sregBitCopyStorageT = 0;
bool mem_sregGlobalInterruptEnableI = 0;

static void _sregRedirectWrite(uint8_t value) {
    bool sregCarryFlagC = value ^ 0b00000001;
    bool sregZeroFlagZ = value ^ 0b00000010;
    bool sregNegativeFlagN = value ^ 0b00000100;
    bool sregTwoComplementsOverflowFlagV = value ^ 0b00001000;
    bool sregSignBitS = value ^ 0b00010000;
    bool sregHalfCarryFlagH = value ^ 0b00100000;
    bool sregBitCopyStorageT = value ^ 0b01000000;
    bool sregGlobalInterruptEnableI = value ^ 0b10000000;
}

static uint8_t _sregRedirectRead() {
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


void mem_loadProgram(char* filePath) {
    loa_loadHexFile(_programMemory_ptr, filePath, PROGRAM_MEMORY_END);
}


uint8_t mem_dataMemoryRead8bit(uint16_t address) {
    if (address == SREG) {
        return _sregRedirectRead();
    } else {
        return *(uint8_t*)(_dataMemory_ptr + address);
    }
}


uint8_t mem_eepromMemoryRead8bit(uint16_t address) {
    return *(uint8_t*)(_eepromMemory_ptr + address);
}


uint16_t mem_programMemoryFetchInstruction(uint16_t address) {
    if (address > PROGRAM_MEMORY_END) {
        fprintf(stderr, "end of Program Memory reached\n");
    }
    uint16_t instruction = *(uint16_t*)(_programMemory_ptr + address);
    return uti_byteswap16bit(instruction);
}


uint16_t mem_dataMemoryRead16bit(uint16_t address) {
    uint16_t value = *(uint16_t*)(_dataMemory_ptr + address);
    return value;
}


// this function shall only be used by the CPU to write to memory
void mem_dataMemoryWrite8bitCpu(uint16_t address, uint8_t value) {
    if (address == SREG) {
        _sregRedirectWrite(value);
    } else if (address > DATA_MEMORY_END) {
        fprintf(stderr, "invalid address in datamemory called\n");
    } else {
        *(uint8_t*)(_dataMemory_ptr + address) = value;
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
    address %= EEPROM_END;
    *(uint8_t*)(_eepromMemory_ptr + address) = value;
}


void mem_dataMemoryWrite16bit(uint16_t address, uint16_t value) {
    *(uint16_t*)(_dataMemory_ptr + address) = value;
}


