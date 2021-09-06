// interface to interact with the emulator

#pragma once

#include <stdbool.h>
#include "memory/memory.h"
#include "cpu/cpu.h"


;
// Public
// ____________________________________________________________________________________________________________________


void atm_loadProgram(char* filePath) {
    mem_loadProgram(filePath);
}

//void atm_setClockSpeed(int clockSpeed) {
//    tim_atmegaClockSpeed = clockSpeed;
//}

void atm_start() {
    cpu_start();
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
