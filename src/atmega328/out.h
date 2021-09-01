// functions for outbound communication live here. Don't access any internals!

#pragma once

#include <stdio.h>
#include "debug.h"


// Public
// ____________________________________________________________________________________________________________________

// everytime an output pin changes it's state this function is called
void out_setPin(uint8_t pinNum, bool isHigh) {
    if (isHigh) {
        printf("Pin %d set high\n", pinNum);
    } else {
        printf("Pin %d set low\n", pinNum);
    }
}



// DEBUGGING
// ____________________________________________________________________________________________________________________


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
#define PRINT_SREG 2
void out_handleSignal(uint8_t val) {
    switch (val) {
        case EMULATION_STOP: exit(0); break;
        case TRIGGER_SPEEDTEST: deb_triggerSpeedTest(); break;
        case PRINT_SREG: deb_print_binary_8bit(arr_dataMemory[SREG]); break;
        default: break;
    }
}

