#pragma once
;
#include <stdint.h>
#include "memory/memory.h"

// pin modes
#define UNUSED 0
#define DIGITAL_IN 1
#define ANALOG_IN 2
#define PWM_IN 3
#define DIGITAL_OUT 1
#define ANALOG_OUT 2
#define PWM_OUT 3


typedef struct pinState {
    int pinNumber;
    uint16_t mode;
    uint16_t value;
    uint16_t freq;
    struct timespec currentTime;
} pinState;


#define QUEUE_SIZE 10
pinState pinChangeQueue[QUEUE_SIZE] = {0};
static int _queueFront = 0;
static int _queueRear = 0;


static bool _currentPinStates[28] = {0};


static bool _pinChangePending = false;


static pinState _dequeuePinChange() {
    _queueFront = (_queueFront + 1) % QUEUE_SIZE;
    pinState pinChange =  pinChangeQueue[_queueFront];
    pinChangeQueue[_queueFront] = (pinState){0, 0, 0, 0, 0};
    if (_queueFront == _queueRear) {
        _pinChangePending = false;
    }
    return pinChange;
}



// Public
// ____________________________________________________________________________________________________________________


void pin_enqueuePinChange(uint16_t pinNumber, uint16_t pinValue) {
    pinState pin = (pinState){pinNumber, 0, pinValue, 0, 0};

    _queueRear = (_queueRear + 1) % QUEUE_SIZE;
    pinChangeQueue[_queueRear] = pin;
    if (_queueFront == _queueRear) {
        _queueFront = (_queueFront + 1) % QUEUE_SIZE;
    }
    _pinChangePending = true;
}


void pin_handlePinChanges() {
    if (_pinChangePending) {
        pinState newPinState = _dequeuePinChange();
        switch (newPinState.pinNumber) {
            case 1: mem_externalSourceWrite8bit(PORTC, mem_dataMemoryRead8bit(PORTC) ^ (1 << PORTC6_PIN_1)); break;
            case 2: mem_externalSourceWrite8bit(PORTD, mem_dataMemoryRead8bit(PORTD) ^ (1 << PORTD0_PIN_2)); break;
            case 3: mem_externalSourceWrite8bit(PORTD, mem_dataMemoryRead8bit(PORTD) ^ (1 << PORTD1_PIN_3)); break;
            case 4: mem_externalSourceWrite8bit(PORTD, mem_dataMemoryRead8bit(PORTD) ^ (1 << PORTD2_PIN_4)); break;
            case 5: mem_externalSourceWrite8bit(PORTD, mem_dataMemoryRead8bit(PORTD) ^ (1 << PORTD3_PIN_5)); break;
            case 6: mem_externalSourceWrite8bit(PORTD, mem_dataMemoryRead8bit(PORTD) ^ (1 << PORTD6_PIN_12)); break;
            case 7: ; break;
            case 8: ; break;
            case 9: mem_externalSourceWrite8bit(PORTB, mem_dataMemoryRead8bit(PORTB) ^ (1 << PORTB6_PIN_9)); break;
            case 10: mem_externalSourceWrite8bit(PORTB, mem_dataMemoryRead8bit(PORTB) ^ (1 << PORTB7_PIN_10)); break;
            case 11: mem_externalSourceWrite8bit(PORTD, mem_dataMemoryRead8bit(PORTD) ^ (1 << PORTD5_PIN_11)); break;
            case 12: mem_externalSourceWrite8bit(PORTD, mem_dataMemoryRead8bit(PORTD) ^ (1 << PORTD6_PIN_12)); break;
            case 13: mem_externalSourceWrite8bit(PORTD, mem_dataMemoryRead8bit(PORTD) ^ (1 << PORTD7_PIN_13)); break;
            case 14: mem_externalSourceWrite8bit(PORTB, mem_dataMemoryRead8bit(PORTB) ^ (1 << PORTB0_PIN_14)); break;
            case 15: mem_externalSourceWrite8bit(PORTB, mem_dataMemoryRead8bit(PORTB) ^ (1 << PORTB1_PIN_15)); break;
            case 16: mem_externalSourceWrite8bit(PORTB, mem_dataMemoryRead8bit(PORTB) ^ (1 << PORTB2_PIN_16)); break;
            case 17: mem_externalSourceWrite8bit(PORTB, mem_dataMemoryRead8bit(PORTB) ^ (1 << PORTB3_PIN_17)); break;
            case 18: mem_externalSourceWrite8bit(PORTB, mem_dataMemoryRead8bit(PORTB) ^ (1 << PORTB4_PIN_18)); break;
            case 19: mem_externalSourceWrite8bit(PORTB, mem_dataMemoryRead8bit(PORTB) ^ (1 << PORTB5_PIN_19)); break;
            case 20: ; break;
            case 21: ; break;
            case 22: mem_externalSourceWrite8bit(PORTC, mem_dataMemoryRead8bit(PORTC) ^ (1 << PORTC0_PIN_22)); break;
            case 23: ; break;
            case 24: mem_externalSourceWrite8bit(PORTC, mem_dataMemoryRead8bit(PORTC) ^ (1 << PORTC1_PIN_24)); break;
            case 25: mem_externalSourceWrite8bit(PORTC, mem_dataMemoryRead8bit(PORTC) ^ (1 << PORTC2_PIN_25)); break;
            case 26: mem_externalSourceWrite8bit(PORTC, mem_dataMemoryRead8bit(PORTC) ^ (1 << PORTC3_PIN_26)); break;
            case 27: mem_externalSourceWrite8bit(PORTC, mem_dataMemoryRead8bit(PORTC) ^ (1 << PORTC4_PIN_27)); break;
            case 28: mem_externalSourceWrite8bit(PORTC, mem_dataMemoryRead8bit(PORTC) ^ (1 << PORTC5_PIN_28)); break;
            default: ; break;
        }
    }
}





