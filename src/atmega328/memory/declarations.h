#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "map.h"



uint16_t mem_programMemory[PROGRAM_MEMORY_END] = {0};
uint8_t mem_dataMemory[DATA_MEMORY_END] = {0};
uint8_t mem_eepromMemory[EEPROM_END] = {0};


uint16_t cpu_programCounter = 0;
long long cpu_cpuCycleCounter = 0;

void cpu_incrementCycleCounter(uint8_t);
void cpu_decrementIncrementStackPointer(int8_t);

bool reg_sregCarry = 0;
bool reg_sregZero = 0;
bool reg_sregNegative = 0;
bool reg_sregTwoComplOverflow = 0;
bool reg_sregSignBit = 0;
bool reg_sregHalfCarry = 0;
bool reg_sregBitCopyStorage = 0;
bool reg_sregGlobalInterruptEnable = 0;