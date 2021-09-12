#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "memory/map.h"
#include "declarations.h"

// logic level shifts
#define LOW 1
#define CHANGING 2
#define FALLING 3
#define RISING 4

//bool _interruptPending = false;

static void _storeProgramCounterOnStack() {
    uint16_t stackTop_addr = mem_dataRead16bit(STACKPOINTER);
    mem_dataWrite16bit(stackTop_addr - 1, reg_programCounter);
    cpu_decrementIncrementStackPointer(-2);
    reg_sregGlobalInterruptEnable = false;
    cpu_incrementCycleCounter(4);
}

// Public
// ____________________________________________________________________________________________________________________

// catch External Interrupt Request 0
void int_catchInt0Interrupt(uint8_t logicLevel) {
    bool isc01 = reg_externalInterruptControl1;
    bool isc00 = reg_externalInterruptControl0;

    if (reg_externalInterruptMask0) {
        if (
            !isc01 && !isc00 && (logicLevel == LOW) ||
            !isc01 && isc00 && (logicLevel == CHANGING) ||
            isc01 && !isc00 && (logicLevel == FALLING) ||
            isc01 && isc00 && (logicLevel == RISING))
        {
            // When an edge or logic change on the INT1 pin triggers an interrupt request, INTF1
            // becomes set (one). The flag is cleared when the interrupt routine is executed.
            reg_externalInterruptFlag0 = true;
        }
    }
}

// catch External Interrupt Request 1
void int_catchInt1Interrupt(uint8_t logicLevel) {
    bool isc11 = reg_externalInterruptControl3;
    bool isc10 = reg_externalInterruptControl2;

    if (reg_externalInterruptMask1) {
        if (
            !isc11 && !isc10 && (logicLevel == LOW) ||
            !isc11 && isc10 && (logicLevel == CHANGING) ||
            isc11 && !isc10 && (logicLevel == FALLING) ||
            isc11 && isc10 && (logicLevel == RISING))
        {
            // When an edge or logic change on the INT1 pin triggers an interrupt request, INTF1
            // becomes set (one). The flag is cleared when the interrupt routine is executed.
            reg_externalInterruptFlag1 = true;
        }
    }
}

// catch Pin Change Interrupt Request 0
void int_catchPcint0Interrupt(uint8_t changedBits) {
    if (reg_pinChangeInterruptControl0) {
        if (changedBits & mem_dataRead8bit(PCMSK0)) {
            reg_pinChangeInterruptFlag0 = true;
        }
    }
}

// catch Pin Change Interrupt Request 1
void int_catchPcint1Interrupt(uint8_t changedBits) {
    if (reg_pinChangeInterruptControl1) {
        if (changedBits & mem_dataRead8bit(PCMSK0)) {
            reg_pinChangeInterruptFlag1 = true;
        }
    }
}

// catch Pin Change Interrupt Request 2
void int_catchPcint2Interrupt(uint8_t changedBits) {
    if (reg_pinChangeInterruptControl2) {
        if (changedBits & mem_dataRead8bit(PCMSK0)) {
            reg_pinChangeInterruptFlag2 = true;
        }
    }
}

void int_handleInterrupts() {
    if (reg_sregGlobalInterruptEnable) {
        // check for for interrupts that trigger on low level
        int_catchInt1Interrupt(LOW);
        int_catchInt0Interrupt(LOW);

        // Execude pending Interrupt
        
        if (reg_externalInterruptFlag0) {
            _storeProgramCounterOnStack();
            reg_programCounter = INT0_INTERRUPT_ADDR;
            reg_externalInterruptFlag0 = false;
        }

        else if (reg_externalInterruptFlag1) {
            _storeProgramCounterOnStack();
            reg_programCounter = INT1_INTERRUPT_ADDR;
            reg_externalInterruptFlag1 = false;
        }

        else if (reg_pinChangeInterruptFlag0) {
            _storeProgramCounterOnStack();
            reg_programCounter = PCINT0_INTERRUPT_ADDR;
            reg_pinChangeInterruptFlag0 = false;
        }

        else if (reg_pinChangeInterruptFlag1) {
            _storeProgramCounterOnStack();
            reg_programCounter = PCINT1_INTERRUPT_ADDR;
            reg_pinChangeInterruptFlag1 = false;
        }

        else if (reg_pinChangeInterruptFlag2) {
            _storeProgramCounterOnStack();
            reg_programCounter = PCINT2_INTERRUPT_ADDR;
            reg_pinChangeInterruptFlag2 = false;
        }
    }
}
