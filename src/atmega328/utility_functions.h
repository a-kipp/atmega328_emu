// All functions in this file are free from side effects. No global variables are used.

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


// Public
// ____________________________________________________________________________________________________________________

uint16_t uti_byteswap16bit(uint16_t num) {
    return num << 8 | num >> 8;
}


bool uti_getBit(uint16_t word, int bit) {
    if (word & (1 << bit)) {
        return true;
    } else {
        false;
    }
}


uint8_t uti_setBitInByte(uint8_t byte, int bit , bool isSet) {
    byte = byte | (isSet << bit);
    return byte;
}


uint8_t uti_setBitInWord(uint16_t word, int bit , bool isSet) {
    word = word | (isSet << bit);
    return word;
}


uint32_t uti_extractBits(uint32_t num, int firstBit, int lastBit) {
    num >>= firstBit;
    uint32_t mask = 0xFFFFFF >> (16 - (lastBit - firstBit));
    return num ^= mask;
}


void uti_printBinary16bit(uint16_t integer_number) {
    for(int i=0; i<16; i++) {
        printf("%d", (integer_number>>15));
        integer_number<<=1;
    }
}

void uti_print_binary_8bit(uint8_t integer_number) {
    for(int i=0; i<8; i++) {
        printf("%d", (integer_number>>7));
        integer_number<<=1;
    }
}


