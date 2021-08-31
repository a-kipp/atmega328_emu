#pragma once

#include "map.h"
#include <stdint.h>
#include <stdbool.h>

// Public
// ____________________________________________________________________________________________________________________


// for easier access flags are stored separatly, readings and writes from SREG register will be intercepted.
bool vir_sregCarryFlagC = 0;
bool vir_sregZeroFlagZ = 0;
bool vir_sregNegativeFlagN = 0;
bool vir_sregTwoComplementsOverflowFlagV = 0;
bool vir_sregSignBitS = 0;
bool vir_sregHalfCarryFlagH = 0;
bool vir_sregBitCopyStorageT = 0;
bool vir_sregGlobalInterruptEnableI = 0;


uint16_t vir_programMemory[PROGRAM_MEMORY_END] = {0};
uint8_t vir_dataMemory[DATA_MEMORY_END] = {0};
uint8_t vir_eepromMemory[EEPROM_END] = {0};
uint16_t *vir_programMemory_ptr = vir_programMemory;
uint8_t *vir_dataMemory_ptr = vir_dataMemory;
uint8_t *vir_eepromMemory_ptr = vir_eepromMemory;
