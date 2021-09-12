// interface to interact with the emulator

#pragma once

#include <stdbool.h>
#include "cpu.h"


;
// Public
// ____________________________________________________________________________________________________________________


void atm_loadProgram(char* filePath) {
    mem_loadProgram(filePath);
}

void atm_setClock(int clockSpeed) {
    cpu_clockSpeed = clockSpeed;
}

void atm_start() {
    cpu_start();
}

void atm_startVerbose() {
    cpu_startVerbose();
}


void atm_stop() {
    cpu_stop();
}

//void atm_setPin(int pinNumber, bool pinValue) {
//    pin_enqueuePinChange(pinNumber, pinValue);
//}

void atm_disassemble() {
    ins_disassembleProgram();
}

void atm_setPin4high() {
    Event pinEvent = {eve_writeToInt0Pin, 1};
    eve_enqueueEvent(pinEvent);
}

void atm_setPin4low() {
    Event pinEvent = {eve_writeToInt0Pin, 0};
    eve_enqueueEvent(pinEvent);
}

void atm_setPin5high() {
    Event pinEvent = {eve_writeToInt1Pin, 1};
    eve_enqueueEvent(pinEvent);
}

void atm_setPin5low() {
    Event pinEvent = {eve_writeToInt1Pin, 0};
    eve_enqueueEvent(pinEvent);
}