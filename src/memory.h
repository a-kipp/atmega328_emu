#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "memory_map.h"
#include "global_variables.h"
#include "utility_functions.h"
#include "loader.h"

uint16_t *_programMemory_ptr;
uint8_t *_dataMemory_ptr;
uint8_t *_eepromMemory_ptr;



void mem_init(char* filePath) {
    _programMemory_ptr = calloc(PROGRAM_MEMORY_END + 1, sizeof(uint8_t));
    _dataMemory_ptr = calloc(DATA_MEMORY_END + 1, sizeof(uint8_t));
    _eepromMemory_ptr = calloc(EEPROM_END + 1, sizeof(uint8_t));
    loa_loadHexFile(_programMemory_ptr, filePath, PROGRAM_MEMORY_END);
}


// Read and Write
// ____________________________________________________________________________________________________________________

uint8_t mem_dataMemoryRead8bit(uint16_t address) {
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

void mem_dataMemoryWrite8bit(uint16_t address, uint8_t value) {
    *(uint8_t*)(_dataMemory_ptr + address) = value;
}

void mem_eepromMemoryWrite8bit(uint16_t address, uint8_t value) {
    *(uint8_t*)(_eepromMemory_ptr + address) = value;
}

void mem_dataMemoryWrite16bit(uint16_t address, uint16_t value) {
    *(uint16_t*)(_dataMemory_ptr + address) = value;
}

bool mem_getSregCarryFlag() {
    return (bool)uti_getBit(mem_dataMemoryRead8bit(SREG), CARRY_FLAG);
}

bool mem_getSregZeroFlag() {
    return (bool)uti_getBit(mem_dataMemoryRead8bit(SREG), ZERO_FLAG);
}

bool mem_getSregNegativeFlag() {
    return (bool)uti_getBit(mem_dataMemoryRead8bit(SREG), NEGATIVE_FLAG);
}

bool mem_getSregTwosComplementOverflowFlag() {
    return (bool)uti_getBit(mem_dataMemoryRead8bit(SREG), TWOS_COMPLEMENT_OVERFLOW_FLAG);
}

bool mem_getSregSignBit() {
    return (bool)uti_getBit(mem_dataMemoryRead8bit(SREG), SIGN_BIT);
}

bool mem_getSregHalfCarryFlag() {
    return (bool)uti_getBit(mem_dataMemoryRead8bit(SREG), HALF_CARRY_FLAG);
}

void mem_setSregCarryFlagTo(bool val) {
    if (val) {
        uti_setBit(_dataMemory_ptr + SREG, CARRY_FLAG);
    } else {
        uti_clearBit(_dataMemory_ptr + SREG, CARRY_FLAG);
    }
}

void mem_setSregZeroFlagTo(bool val) {
    if (val) {
        uti_setBit(_dataMemory_ptr + SREG, ZERO_FLAG);
    } else {
        uti_clearBit(_dataMemory_ptr + SREG, ZERO_FLAG);
    }
}

void mem_setSregNegativeFlagTo(bool val) {
    if (val) {
        uti_setBit(_dataMemory_ptr + SREG, NEGATIVE_FLAG);
    } else {
        uti_clearBit(_dataMemory_ptr + SREG, NEGATIVE_FLAG);
    }
}

void mem_setSregTwosComplementOverflowFlagTo(bool val) {
    if (val) {
        uti_setBit(_dataMemory_ptr + SREG, TWOS_COMPLEMENT_OVERFLOW_FLAG);
    } else {
        uti_clearBit(_dataMemory_ptr + SREG, TWOS_COMPLEMENT_OVERFLOW_FLAG);
    }
}

void mem_setSregSignBitTo(bool val) {
    if (val) {
        uti_setBit(_dataMemory_ptr + SREG, SIGN_BIT);
    } else {
        uti_clearBit(_dataMemory_ptr + SREG, SIGN_BIT);
    }
}

void mem_setSregHalfCarryFlagTo(bool val) {
    if (val) {
        uti_setBit(_dataMemory_ptr + SREG, HALF_CARRY_FLAG);
    } else {
        uti_clearBit(_dataMemory_ptr + SREG, HALF_CARRY_FLAG);
    }
}