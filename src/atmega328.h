// interface to interact with the emulator

#include <stdbool.h>
#include "atmega328/memory.h"
#include "atmega328/cpu.h"
#include "atmega328/pin.h"

#pragma once

;
// Public
// ____________________________________________________________________________________________________________________


void atm_init() {
    mem_init();
}

void atm_loadProgram(char* filePath) {
    mem_loadProgram(filePath);
}

void atm_setClock() {

}

void atm_start() {
    cpu_start();
}

void atm_stop() {
    cpu_stop();
}

void atm_setPin(uint16_t pinNumber, uint16_t pinValue) {
    pin_enqueuePinChange(pinNumber, pinValue);
}



// Accessing Peripherals
// ____________________________________________________________________________________________________________________

// everytime an output pin changes it's state this function is called
void atm_onPinChange(PinConfiguration pin) {
    // attached peripherals are called here
}

