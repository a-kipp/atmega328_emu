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
uint16_t reg_programCounter = 0;
long long cpu_cycleCounter = 0;
void cpu_incrementCycleCounter(uint8_t);
void cpu_decrementIncrementStackPointer(int8_t);


void (*jti_implementationTable[])(void);


bool reg_sregCarry = false;
bool reg_sregZero = false;
bool reg_sregNegative = false;
bool reg_sregTwoComplOverflow = false;
bool reg_sregSignBit = false;
bool reg_sregHalfCarry = false;
bool reg_sregBitCopyStorage = false;
bool reg_sregGlobalInterruptEnable = false;

bool reg_pinChangeInterruptFlag2 = false;
bool reg_pinChangeInterruptFlag1 = false;
bool reg_pinChangeInterruptFlag0 = false;

bool reg_externalInterruptFlag1  = false;
bool reg_externalInterruptFlag0  = false;

bool reg_externalInterruptMask1 = false;
bool reg_externalInterruptMask0 = false;

bool reg_externalInterruptControl3 = false;
bool reg_externalInterruptControl2 = false;
bool reg_externalInterruptControl1 = false;
bool reg_externalInterruptControl0 = false;

bool reg_pinChangeInterruptControl2 = false;
bool reg_pinChangeInterruptControl1 = false;
bool reg_pinChangeInterruptControl0 = false;

