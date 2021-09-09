#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "map.h"
#include "utils.h"
#include "register.h"
#include "arrays.h"
#include "declarations.h"



static void _callInterrupt(uint16_t interruptVector_addr) {
    uint16_t stackTop_addr = arr_get16BitDataArray(STACKPOINTER);
    reg_sregGlobalInterruptEnable = false;
    arr_store16BitDataArray(stackTop_addr - 1, cpu_programCounter);
    cpu_decrementIncrementStackPointer(-2);
    cpu_programCounter = interruptVector_addr;
    cpu_incrementCycleCounter(4);
}



// External interrupt request 1

#define LOW 0
#define CHANGING 1
#define FALLING 2
#define RISING 3


static void _execudeExternalInterruptRequest1() {
    reg_sregGlobalInterruptEnable = false;
    cpu_programCounter = 0x0004;
    uint8_t eifrContent = arr_get8BitDataArray(EIFR);
    eifrContent = uti_setBitInByte(eifrContent, INTF1, false);
    arr_store8BitDataArray(EIFR, eifrContent);
}


void int_matchExternalInterruptRequest1(uint8_t logicLevel) {

    uint8_t eimskContent = arr_get8BitDataArray(EIMSK);
    uint8_t eicraContent = arr_get8BitDataArray(EICRA);
    uint8_t eifrContent = arr_get8BitDataArray(EIFR);
    bool isc11 = uti_getBit(eicraContent, ISC11);
    bool isc10 = uti_getBit(eicraContent, ISC10);

    if (uti_getBit(eimskContent, INT1)) {
        if (
            !isc11 && !isc10 && logicLevel == LOW || 
            !isc11 && isc10 && logicLevel == CHANGING ||
            isc11 && !isc10 && logicLevel == FALLING ||
            isc11 && isc10 && logicLevel == RISING
        ) {
            eifrContent = uti_setBitInByte(eifrContent, INTF1, true);
            arr_store8BitDataArray(EIFR, eifrContent);
        }
    }
    if (uti_getBit(eifrContent, INTF1) && reg_sregGlobalInterruptEnable) {
        _execudeExternalInterruptRequest1();
    }
}





