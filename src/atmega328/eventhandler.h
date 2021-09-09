#pragma once

#include "memory/memory.h"


#define QUEUE_SIZE 100

typedef struct Event {
    void (*fun_ptr)(uint8_t);
    uint8_t argument;
} Event;
;

bool _eventPending = true;


Event eventQueue[QUEUE_SIZE] = {0};
int _queueFront = 0;
int _queueRear = 0;



void eve_enqueueEvent(Event event) {
    _queueRear = (_queueRear + 1 + QUEUE_SIZE) % QUEUE_SIZE;
    eventQueue[_queueRear] = event;
    if (_queueFront == _queueRear) {
        _queueFront = (_queueFront + 1 + QUEUE_SIZE) % QUEUE_SIZE;
    }
    _eventPending = true;
}


Event _dequeueEvent() {
    _queueFront = (_queueFront + 1 + QUEUE_SIZE) % QUEUE_SIZE;
    Event event =  eventQueue[_queueFront];
    if (_queueFront == _queueRear) {
        _eventPending = false;
    }
    return event;
}



void eve_setPortB(uint8_t bitField) {
    acc_dataWrite8bit(PORTB, bitField);
}


void eve_setPortC(uint8_t bitField) {
    acc_dataWrite8bit(PORTC, bitField);
}


void eve_setPortD(uint8_t bitField) {
    acc_dataWrite8bit(PORTD, bitField);
}


void eve_setUdr0(uint8_t bitField) {
    acc_dataWrite8bit(UDR0, bitField);
}


void eve_handleEvent() {
}
