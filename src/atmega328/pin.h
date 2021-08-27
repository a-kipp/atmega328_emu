#pragma once
;
#include <stdint.h>
#include "timing.h"


// pin modes
#define UNUSED 0
#define DIGITAL_IN 1
#define ANALOG_IN 2
#define PWM_IN 3
#define DIGITAL_OUT 1
#define ANALOG_OUT 2
#define PWM_OUT 3


typedef struct PinConfiguration {
    uint16_t number;
    uint16_t mode;
    uint16_t value;
    uint16_t freq;
    int timestamp;
} PinConfiguration;


#define QUEUE_SIZE 1000
static int _queueFront = 0;
static int _queueRear = 0;


// Public
// ____________________________________________________________________________________________________________________

PinConfiguration pinChangeQueue[QUEUE_SIZE] = {0};

bool pin_pinChangePending = false;

void pin_enqueuePinChange(uint16_t pinNumber, uint16_t pinValue) {
    PinConfiguration pin = (PinConfiguration){pinNumber, 0, pinValue, 0, tim_getTime()};

    _queueRear = (_queueRear + 1) % QUEUE_SIZE;
    pinChangeQueue[_queueRear] = pin;
    if (_queueFront == _queueRear) {
        _queueFront = (_queueFront + 1) % QUEUE_SIZE;
    }
    pin_pinChangePending = true;
}


PinConfiguration pin_dequeuePinChange() {
    _queueFront = (_queueFront + 1) % QUEUE_SIZE;
    PinConfiguration pinChange =  pinChangeQueue[_queueFront];
    pinChangeQueue[_queueFront] = (PinConfiguration){0, 0, 0, 0, 0};
    if (_queueFront == _queueRear) {
        pin_pinChangePending = false;
    }
    return pinChange;
}


