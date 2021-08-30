// functions for outbound communication live here. Don't access any internals!

#pragma once

#include <stdio.h>
#include "debug.h"
#include "timing.h"


// Public
// ____________________________________________________________________________________________________________________

// everytime an output pin changes it's state this function is called
void out_pinChange() {
    // attached peripherals are called here
}

void out_serialOutBin(uint8_t val) {
    setvbuf (stdout, NULL, _IONBF, 0);
    deb_print_binary_8bit(val);
// TODO set stdout buffer to correct value
    setvbuf (stdout, NULL, _IONBF, 1);
}

void out_serialOutChar(uint8_t val) {
    setvbuf (stdout, NULL, _IONBF, 0);
    printf("%c", val);
// TODO set stdout buffer to correct value
    setvbuf (stdout, NULL, _IONBF, 1);
}

void out_serialOutDec(uint8_t val) {
    setvbuf (stdout, NULL, _IONBF, 0);
    printf("%d", val);
// TODO set stdout buffer to correct value
    setvbuf (stdout, NULL, _IONBF, 1);
}

void out_serialOutHex(uint8_t val) {
    setvbuf (stdout, NULL, _IONBF, 0);
    printf("%02X", val);
// TODO set stdout buffer to correct value
    setvbuf (stdout, NULL, _IONBF, 1);
}

#define EMULATION_STOP 0
#define TRIGGER_SPEEDTEST 1
void out_handleSignal(uint8_t val) {
    switch (val) {
        case EMULATION_STOP: exit(0); break;
        case TRIGGER_SPEEDTEST: tim_triggerSpeedTest(); break;
    }
}
