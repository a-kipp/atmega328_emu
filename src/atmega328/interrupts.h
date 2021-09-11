#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "memory/map.h"
#include "declarations.h"

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


// logic level shifts
#define LOW 0
#define CHANGING 1
#define FALLING 2
#define RISING 3



bool _interruptPending = false;
bool _activeInterrupts[TOATAL_NUMBER_INTERRUPTS];

static void _prepareInterruptCall() {
    uint16_t stackTop_addr = mem_dataRead16bit(STACKPOINTER);
    mem_dataWrite16bit(stackTop_addr - 1, cpu_programCounter);
    cpu_decrementIncrementStackPointer(-2);
    reg_sregGlobalInterruptEnable = false;
    cpu_incrementCycleCounter(4);
}




// Public
// ____________________________________________________________________________________________________________________



// match INT0_INTERRUPT
void int_matchExternalInterruptRequest0(uint8_t logicLevel) {

    bool isc01 = uti_getBit(mem_dataMemory[EICRA], ISC01);
    bool isc00 = uti_getBit(mem_dataMemory[EICRA], ISC00);

    if (uti_getBit(mem_dataMemory[EIMSK], INT0)) {
        if (
            !isc01 && !isc00 && (logicLevel == LOW) || 
            !isc01 && isc00 && (logicLevel == CHANGING) ||
            isc01 && !isc00 && (logicLevel == FALLING) ||
            isc01 && isc00 && (logicLevel == RISING)
        ) {
            // When an edge or logic change on the INT1 pin triggers an interrupt request, INTF1
            // becomes set (one). The flag is cleared when the interrupt routine is executed.
            mem_dataMemory[EIFR] = uti_setBit(mem_dataMemory[EIFR], INTF0, true);
        }
    }
    if (uti_getBit(mem_dataMemory[EIFR], INTF0)) {
        _activeInterrupts[INT0_INTERRUPT] = true;
        _interruptPending = true;
    }
}



// match INT1_INTERRUPT
void int_matchExternalInterruptRequest1(uint8_t logicLevel) {

    bool isc11 = uti_getBit(mem_dataMemory[EICRA], ISC11);
    bool isc10 = uti_getBit(mem_dataMemory[EICRA], ISC10);

    if (uti_getBit(mem_dataMemory[EIMSK], INT1)) {
        if (
            !isc11 && !isc10 && (logicLevel == LOW) || 
            !isc11 && isc10 && (logicLevel == CHANGING) ||
            isc11 && !isc10 && (logicLevel == FALLING) ||
            isc11 && isc10 && (logicLevel == RISING)
        ) {
            // When an edge or logic change on the INT1 pin triggers an interrupt request, INTF1
            // becomes set (one). The flag is cleared when the interrupt routine is executed.
            mem_dataMemory[EIFR] = uti_setBit(mem_dataMemory[EIFR], INTF1, true);
        }
    }
    if (uti_getBit(mem_dataMemory[EIFR], INTF1)) {
        _activeInterrupts[INT1_INTERRUPT] = true;
        _interruptPending = true;
    }
}





void int_handleInterrupts() {

    if (reg_sregGlobalInterruptEnable) {

        // check for for interrupts that trigger on low level
        int_matchExternalInterruptRequest1(LOW);
        int_matchExternalInterruptRequest0(LOW);

        if (_interruptPending) {

            // INT0_INTERRUPT
            if (uti_getBit(mem_dataRead8bit(EIFR), INTF0)) {
                void _prepareInterruptCall();
                cpu_programCounter = INT0_INTERRUPT;
                uint8_t eifrContent = mem_dataRead8bit(EIFR);
                eifrContent = uti_setBit(eifrContent, INTF0, false);
                mem_dataWrite8bit(EIFR, eifrContent);
            }


            // INT1_INTERRUPT
            else if (uti_getBit(mem_dataRead8bit(EIFR), INTF1)) {
                _prepareInterruptCall();
                cpu_programCounter = INT1_INTERRUPT;
                uint8_t eifrContent = mem_dataRead8bit(EIFR);
                eifrContent = uti_setBit(eifrContent, INTF1, false);
                mem_dataWrite8bit(EIFR, eifrContent);
            }

            
            else {
                _interruptPending = false;
            }

            // When the AVR exits from an interrupt, it will always return to the main program and
            // execute one more instruction before any pending interrupt is served.
        }
    }
}   
