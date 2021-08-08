#include <stdint.h>
#include <stdlib.h>
#include "memory_map.h"
#include "global_variables.h"
#include "utility_functions.h"

uint16_t *_programMemory_ptr;
uint8_t *_dataMemory_ptr;
uint8_t *_eepromMemory_ptr;



void mem_init(char* FilePath) {
    _programMemory_ptr = calloc(PROGRAM_MEMORY_END + 1, sizeof(uint8_t));
    _dataMemory_ptr = calloc(DATA_MEMORY_END + 1, sizeof(uint8_t));
    _eepromMemory_ptr = calloc(EEPROM_END + 1, sizeof(uint8_t));

    uti_loadFile(_programMemory_ptr, FilePath, PROGRAM_MEMORY_END);
    printf("%s\n", FilePath);
    printf("%s\n", _programMemory_ptr);
}


// Read and Write
// ____________________________________________________________________________________________________________________

uint8_t mem_programMemoryRead8bit(uint16_t address) {
    return *(uint8_t*)(_programMemory_ptr + address);
}

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

void mem_getSregFlag(int flag) {
    uti_extractSingleBit(_dataMemory_ptr + SREG, flag);
}

void mem_setSregFlag(int flag) {
    uti_setSingleBit(_dataMemory_ptr + SREG, flag);
}

void mem_clearSregFlag(int flag) {
    uti_clearSingleBit(_dataMemory_ptr + SREG, flag);
}