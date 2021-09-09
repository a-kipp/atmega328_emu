#pragma once
;
#include <stdint.h>
#include "memory/memory.h"

#define NUMBER_OF_PINS 29


// pin modes
#define UNDEFINED 0
#define DIGITAL_IN 1
#define DIGITAL_OUT 2


typedef struct PinState {
    uint16_t number;
    uint16_t value;
    uint8_t mode;
} PinState;


PinState _pinsByNumber[29] = {0};

PinState *_pinsByAddress[PORTD + 1][8];











#define QUEUE_SIZE 10
PinState pinChangeQueue[QUEUE_SIZE] = {0};
static int _queueFront = 0;
static int _queueRear = 0;


static bool _currentPinStates[28] = {0};


static bool _pinChangePending = false;


static PinState _dequeuePinChange() {
    _queueFront = (_queueFront + 1) % QUEUE_SIZE;
    PinState pinChange =  pinChangeQueue[_queueFront];
    pinChangeQueue[_queueFront] = (PinState){0, 0, 0};
    if (_queueFront == _queueRear) {
        _pinChangePending = false;
    }
    return pinChange;
}


static void _setBitInRegisterTo(uint16_t address, int bitNum, bool isSet) {
    if (isSet) {
        mem_dataWrite8bitFromExtern(address, mem_dataRead8bit(address) | (1 << bitNum));
    } else {
        mem_dataWrite8bitFromExtern(address, mem_dataRead8bit(address) ^ (1 << bitNum));
    }
}


// Public
// ____________________________________________________________________________________________________________________


void pin_enqueuePinChange(uint16_t pinNumber, uint16_t pinValue) {
    PinState pin = (PinState){pinNumber, 0, pinValue};

    _queueRear = (_queueRear + 1) % QUEUE_SIZE;
    pinChangeQueue[_queueRear] = pin;
    if (_queueFront == _queueRear) {
        _queueFront = (_queueFront + 1) % QUEUE_SIZE;
    }
    _pinChangePending = true;
}


void pin_handlePinChanges() {
    if (_pinChangePending) {
        PinState newPinState = _dequeuePinChange();
        switch (newPinState.number) {
            case 1: _setBitInRegisterTo(PORTC, PORTC6_PIN_1, (bool)newPinState.value); break;
            case 2: _setBitInRegisterTo(PORTD, PORTD0_PIN_2, (bool)newPinState.value); break;
            case 3: _setBitInRegisterTo(PORTD, PORTD1_PIN_3, (bool)newPinState.value); break;
            case 4: {
                int_matchExternalInterruptRequest1(LOW)
                break;
            }
            case 5: _setBitInRegisterTo(PORTD, PORTD3_PIN_5, (bool)newPinState.value); break;
            case 6: _setBitInRegisterTo(PORTD, PORTD6_PIN_12, (bool)newPinState.value); break;
            case 7: ; break;
            case 8: ; break;
            case 9: _setBitInRegisterTo(PORTB, PORTB6_PIN_9 , (bool)newPinState.value); break;
            case 10: _setBitInRegisterTo(PORTB, PORTB7_PIN_10, (bool)newPinState.value); break;
            case 11: _setBitInRegisterTo(PORTD, PORTD5_PIN_11, (bool)newPinState.value); break;
            case 12: _setBitInRegisterTo(PORTD, PORTD6_PIN_12, (bool)newPinState.value); break;
            case 13: _setBitInRegisterTo(PORTD, PORTD7_PIN_13, (bool)newPinState.value); break;
            case 14: _setBitInRegisterTo(PORTB, PORTB0_PIN_14, (bool)newPinState.value); break;
            case 15: _setBitInRegisterTo(PORTB, PORTB1_PIN_15, (bool)newPinState.value); break;
            case 16: _setBitInRegisterTo(PORTB, PORTB2_PIN_16, (bool)newPinState.value); break;
            case 17: _setBitInRegisterTo(PORTB, PORTB3_PIN_17, (bool)newPinState.value); break;
            case 18: _setBitInRegisterTo(PORTB, PORTB4_PIN_18, (bool)newPinState.value); break;
            case 19: _setBitInRegisterTo(PORTB, PORTB5_PIN_19, (bool)newPinState.value); break;
            case 20: ; break;
            case 21: ; break;
            case 22: _setBitInRegisterTo(PORTC, PORTC0_PIN_22, (bool)newPinState.value); break;
            case 23: ; break;
            case 24: _setBitInRegisterTo(PORTC, PORTC1_PIN_24, (bool)newPinState.value); break;
            case 25: _setBitInRegisterTo(PORTC, PORTC2_PIN_25, (bool)newPinState.value); break;
            case 26: _setBitInRegisterTo(PORTC, PORTC3_PIN_26, (bool)newPinState.value); break;
            case 27: _setBitInRegisterTo(PORTC, PORTC4_PIN_27, (bool)newPinState.value); break;
            case 28: _setBitInRegisterTo(PORTC, PORTC5_PIN_28, (bool)newPinState.value); break;
            default: ; break;
        }
    }
}





