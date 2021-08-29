// functions for outbound communication live here. Don't access any internals!

#pragma once

#include <stdio.h>


// Public
// ____________________________________________________________________________________________________________________

// everytime an output pin changes it's state this function is called
void out_pinChange() {
    // attached peripherals are called here
}

void out_serialOut(uint8_t c) {
    setvbuf (stdout, NULL, _IONBF, 0);
    printf("%c", c);
// TODO set stdout buffer to correct value
    setvbuf (stdout, NULL, _IONBF, 1);
}

