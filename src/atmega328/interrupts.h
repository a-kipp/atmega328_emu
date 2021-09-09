#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "memory/map.h"
#include "memory/declarations.h"

#define TOATAL_NUMBER_INTERRUPTS 26

#define RESET_INTERRUPT 0
#define INT0_INTERRUPT 1
#define INT1_INTERRUPT 2
#define PCINT0_INTERRUPT 3
#define PCINT1_INTERRUPT 4
#define PCINT2_INTERRUPT 5
#define WDT_INTERRUPT 6
#define TIMER2_COMPA_INTERRUPT 7
#define TIMER2_COMPB_INTERRUPT 8
#define TIMER2_OVF_INTERRUPT 9
#define TIMER1_CAPT_INTERRUPT 10
#define TIMER1_COMPA_INTERRUPT 11
#define TIMER1_COMPB_INTERRUPT 12
#define TIMER1_OVF_INTERRUPT 13
#define TIMER0_COMPA_INTERRUPT 14
#define TIMER0_COMPB_INTERRUPT 15
#define TIMER0_OVF_INTERRUPT 16
#define SPI_STC_INTERRUPT 17
#define USART_RX_INTERRUPT 18
#define USART_UDRE_INTERRUPT 19
#define USART_TX_INTERRUPT 20
#define ADC_INTERRUPT 21
#define EE_READY_INTERRUPT 22
#define ANALOG_COMP_INTERRUPT 23
#define TWI_INTERRUPT 24
#define SPM_READY_INTERRUPT 25

bool _interruptPending = false;
bool _activeInterrupts[TOATAL_NUMBER_INTERRUPTS];


static void _execudeExternalInterruptRequest1() {
    cpu_programCounter = 0x0004;
    mem_dataMemory[EIFR] = uti_setBit(mem_dataMemory[EIFR], INTF1, false);
}


static void _notImplementet() {
    fprintf(stderr, "the called interrupt is not inmplemented\n");
    exit(1);
}


static void(*_interruptCallFunctions[])() = {
    _notImplementet,
    _notImplementet,
    _execudeExternalInterruptRequest1,
};




// Public
// ____________________________________________________________________________________________________________________



void int_handleInterrupts() {
    if (_interruptPending) {

        if (reg_sregGlobalInterruptEnable) {

            for(uint8_t i = 0; i < TOATAL_NUMBER_INTERRUPTS; i++) {

                if (_activeInterrupts[i]) {

                    uint16_t stackTop_addr = arr_get16BitDataArray(STACKPOINTER);

                    mem_dataMemory[stackTop_addr - 1] = cpu_programCounter;

                    cpu_decrementIncrementStackPointer(-2);

                    reg_sregGlobalInterruptEnable = false;

                    _activeInterrupts[i] = false;

                    cpu_incrementCycleCounter(4);

                    _interruptCallFunctions[i];

                }
            }
            _interruptPending = false;
        }
    }
}   





// External interrupt request 1

#define LOW 0
#define CHANGING 1
#define FALLING 2
#define RISING 3


void int_matchExternalInterruptRequest1(uint8_t logicLevel) {

    bool isc11 = uti_getBit(mem_dataMemory[EICRA], ISC11);
    bool isc10 = uti_getBit(mem_dataMemory[EICRA], ISC10);

    if (uti_getBit(mem_dataMemory[EIMSK], INT1)) {
        if (
            !isc11 && !isc10 && logicLevel == LOW || 
            !isc11 && isc10 && logicLevel == CHANGING ||
            isc11 && !isc10 && logicLevel == FALLING ||
            isc11 && isc10 && logicLevel == RISING
        ) {
            mem_dataMemory[EIFR] = uti_setBit(mem_dataMemory[EIFR], INTF1, true);
        }
    }
    if (uti_getBit(mem_dataMemory[EIFR], INTF1) && reg_sregGlobalInterruptEnable) {
        _activeInterrupts[INT1_INTERRUPT] = true;
        _interruptPending = true;
    }
}


