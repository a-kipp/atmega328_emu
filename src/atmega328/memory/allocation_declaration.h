#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "map.h"



// for easier access flags are stored separatly, readings and writes from SREG register will be intercepted.
bool mem_sregCarry = false;
bool mem_sregZero = false;
bool mem_sregNegative = false;
bool mem_sregTwoComplOverflow = false;
bool mem_sregSignBit = false;
bool mem_sregHalfCarry = false;
bool mem_sregBitCopyStorageT = false;
bool mem_sregGlobalInterruptEnableI = false;


uint16_t _programMemory[PROGRAM_MEMORY_END] = {0};
uint8_t _dataMemory[DATA_MEMORY_END] = {0};
uint8_t _eepromMemory[EEPROM_END] = {0};


uint16_t mem_programCounter = 0;
long long mem_cpuCycleCount = 0;