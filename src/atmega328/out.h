#pragma once

#include "pin.h"
#include <stdio.h>

// Accessing Peripherals
// ____________________________________________________________________________________________________________________

// everytime an output pin changes it's state this function is called
void atm_onPinChange(PinConfiguration pin) {
    // attached peripherals are called here
}

void atm_serialOut(uint8_t c) {

setvbuf (stdout, NULL, _IONBF, 0);
    printf("%c", c);
}

