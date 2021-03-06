#pragma once

#include "memory/memory.h"


#define QUEUE_SIZE 100

typedef struct Event {
    void (*fun_ptr)(uint8_t);
    uint8_t argument;
} Event;
;

bool _eventPending = false;


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



void eve_writeToInt0Pin(uint8_t highOrLow) {
    uint8_t pinDContent = mem_dataRead8bit(PIND);
    if (highOrLow == 0) {
        pinDContent = uti_setBit(pinDContent, PIND2_PIN_4, false);
    } else {
        pinDContent = uti_setBit(pinDContent, PIND2_PIN_4, true);
    }
    mem_dataWrite8bit(PIND, pinDContent);
}



void eve_writeToInt1Pin(uint8_t highOrLow) {
    uint8_t pinDContent = mem_dataRead8bit(PIND);
    if (highOrLow == 0) {
        pinDContent = uti_setBit(pinDContent, PIND3_PIN_5, false);
    } else {
        pinDContent = uti_setBit(pinDContent, PIND3_PIN_5, true);
    }
    mem_dataWrite8bit(PIND, pinDContent);
}



void eve_setPortB(uint8_t bitField) {
    mem_dataWrite8bit(PORTB, bitField);
}


void eve_setPortC(uint8_t bitField) {
    mem_dataWrite8bit(PORTC, bitField);
}


void eve_setPortD(uint8_t bitField) {
    mem_dataWrite8bit(PORTD, bitField);
}


void eve_setUdr0(uint8_t bitField) {
    mem_dataWrite8bit(UDR0, bitField);
}


void eve_handleEvents() {
    if (_eventPending) {
        Event event = _dequeueEvent();
        event.fun_ptr(event.argument);
    }
}
