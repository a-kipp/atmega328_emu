#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "memory/map.h"



uint16_t mem_programMemory[PROGRAM_MEMORY_END] = {0};
uint8_t mem_dataMemory[DATA_MEMORY_END] = {0};
uint8_t mem_eepromMemory[EEPROM_END] = {0};
uint16_t mem_fetchInstruction(uint16_t address);
uint8_t mem_dataRead8bit(uint16_t address);
uint16_t mem_dataRead16bit(uint16_t address);
void mem_dataWrite16bit(uint16_t address, uint16_t value);
void mem_dataWrite8bit(uint16_t address, uint8_t value);


long long cpu_clockSpeed = 16000000;
uint16_t cpu_programCounter = 0;
long long cpu_cpuCycleCounter = 0;
void cpu_incrementCycleCounter(uint8_t);
void cpu_decrementIncrementStackPointer(int8_t);


void (*jti_implementationTable[])(void);


bool reg_sregCarry = 0;
bool reg_sregZero = 0;
bool reg_sregNegative = 0;
bool reg_sregTwoComplOverflow = 0;
bool reg_sregSignBit = 0;
bool reg_sregHalfCarry = 0;
bool reg_sregBitCopyStorage = 0;
bool reg_sregGlobalInterruptEnable = 0;