#pragma once

#include "map.h"
#include <stdint.h>
#include <stdbool.h>

// Public
// ____________________________________________________________________________________________________________________


// for easier access flags are stored separatly, readings and writes from SREG register will be intercepted.
bool arr_sregCarryFlagC = 0;
bool arr_sregZeroFlagZ = 0;
bool arr_sregNegativeFlagN = 0;
bool arr_sregTwoComplementsOverflowFlagV = 0;
bool arr_sregSignBitS = 0;
bool arr_sregHalfCarryFlagH = 0;
bool arr_sregBitCopyStorageT = 0;
bool arr_sregGlobalInterruptEnableI = 0;


uint16_t arr_programMemory[PROGRAM_MEMORY_END] = {0};
uint8_t arr_dataMemory[DATA_MEMORY_END] = {0};
uint8_t arr_eepromMemory[EEPROM_END] = {0};
uint16_t *vir_programMemory_ptr = arr_programMemory;
uint8_t *vir_dataMemory_ptr = arr_dataMemory;
uint8_t *vir_eepromMemory_ptr = arr_eepromMemory;
