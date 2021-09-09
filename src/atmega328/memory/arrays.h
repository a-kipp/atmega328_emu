#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "map.h"
#include "declarations.h"







// Public
// ____________________________________________________________________________________________________________________



void arr_store8BitDataArray(uint16_t address, uint8_t value) {
    mem_dataMemory[address] = value;
}



uint8_t arr_get8BitDataArray(uint16_t address) {
    return mem_dataMemory[address];
}



void arr_store16BitDataArray(uint16_t address, uint16_t value) {
    arr_store8BitDataArray(address, (uint8_t)value);
    arr_store8BitDataArray(address + 1, (uint8_t)(value >> 8));
}



uint16_t arr_get16BitDataArray(uint16_t address) {
    uint16_t returnValue = arr_get8BitDataArray(address);
    returnValue |= ((uint16_t)arr_get8BitDataArray(address + 1)) << 8;
    return returnValue;
}





