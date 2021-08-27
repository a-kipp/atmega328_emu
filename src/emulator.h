// interface to interact with the emulator

#include <stdbool.h>
#include "memory.h"
#include "cpu.h"

#pragma once

;
unsigned int inp_interruptSignal = 0;

void emu_init() {
    mem_init();
}

void emu_loadProgram(char* filePath) {
    mem_loadProgram(filePath);
}

void emu_setClock() {

}

void emu_start() {
    cpu_start();
}

void emu_stop() {
    cpu_stop();
}

void emu_setPin0To(bool val) {

}

