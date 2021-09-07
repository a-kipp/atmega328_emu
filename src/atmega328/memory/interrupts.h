#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "memory/map.h"
#include "memory/allocation_declaration.h"
#include "utils.h"



void _callInterrupt(uint16_t interuptVector_addr) {
}




// External interrupt request 0 & 1

#define LOW 0
#define CHANGING 1
#define FALLING 2
#define RISING 3

static void _execudeExternalInterruptRequest1() {
    mem_sregGlobalInterruptEnableI = false;
    mem_programCounter = 0x0004;
}

void int_matchExternalInterruptRequest1(uint8_t logicLevel) {
    bool int1 = uti_getBit(_dataMemory[EIMSK], INT1);
    bool isc11 = uti_getBit(_dataMemory[EICRA], ISC11);
    bool isc10 = uti_getBit(_dataMemory[EICRA], ISC10);

    if (mem_sregGlobalInterruptEnableI && int1) {
        if (!isc11 && !isc10 && logicLevel == LOW) || 
            (!isc11 && isc10 && logicLevel == CHANGING) ||
            (isc11 && !isc10 && logicLevel == FALLING) ||
            (isc11 && isc10 && logicLevel == RISING) {
            execudeExternalInterruptRequest1();
        }
    }
}

static void _execudeExternalInterruptRequest0() {
    mem_sregGlobalInterruptEnableI = false;
    mem_programCounter = 0x0002;
}

void int_matchExternalInterruptRequest0(uint8_t logicLevel) {
    bool int0 = uti_getBit(_dataMemory[EIMSK], INT0);
    bool isc01 = uti_getBit(_dataMemory[EICRA], ISC01);
    bool isc00 = uti_getBit(_dataMemory[EICRA], ISC00);

    if (mem_sregGlobalInterruptEnableI && int0) {
        if (!isc01 && !isc00 && logicLevel == LOW) ||
            (!isc01 && isc00 && logicLevel == CHANGING) ||
            (isc01 && !isc00 && logicLevel == FALLING) ||
            (isc01 && isc00 && logicLevel == RISING) {
            execudeExternalInterruptRequest0();
        }
    }
}



void int_matchNoneTriggerEventInterrupts() {
    matchExternalInterruptRequest1(LOW);
    matchExternalInterruptRequest0(LOW);
} 