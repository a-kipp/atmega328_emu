#pragma once


#define QUEUE_SIZE 10

typedef enum {
    SET_BITS_IN_PINREGISTER,
    RESET_BITS_IN_PINREGISTER
    WRITE_TO_PINREGISTER,
} InputType;


union Value {
   uint16_t value16bit;
   uint8_t value8bit;
};

typedef struct Input {
    InputType inputType;
    Value value;
} Input;


Input pinChangeQueue[QUEUE_SIZE] = {0};
static int _queueFront = 0;
static int _queueRear = 0;


static bool _currentPinStates[28] = {0};


static bool _pinChangePending = false;



void que_enqueueInput(Input input) {
    PinState pin = (PinState){pinNumber, 0, pinValue, 0, 0};

    _queueRear = (_queueRear + 1) % QUEUE_SIZE;
    pinChangeQueue[_queueRear] = pin;
    if (_queueFront == _queueRear) {
        _queueFront = (_queueFront + 1) % QUEUE_SIZE;
    }
    _pinChangePending = true;
}


Input que_dequeueInput() {
    _queueFront = (_queueFront + 1) % QUEUE_SIZE;
    PinState pinChange =  pinChangeQueue[_queueFront];
    pinChangeQueue[_queueFront] = (PinState){0, 0, 0, 0, 0};
    if (_queueFront == _queueRear) {
        _pinChangePending = false;
    }
    return pinChange;
}

bool que_isEmpty() {

}