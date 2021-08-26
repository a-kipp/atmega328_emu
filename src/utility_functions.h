// All functions in this file are free from side effects. No global variables are used.
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


uint16_t uti_byteswap16bit(uint16_t num) {
    return num << 8 | num >> 8;
}

uint8_t uti_getBit(uint8_t byte, int bit) {
    bit = 1 << bit;
    return(bit & byte);
}

void uti_setBit(uint8_t *byte, int bit) {
    bit = 1 << bit;
    *byte = *byte | bit;
}

void uti_clearBit(uint8_t *byte, int bit) {
    bit = 1 << bit;
    bit ^= 0xff;
    *byte = *byte & bit;
}

uint32_t uti_extractBits(uint32_t num, int firstBit, int lastBit) {
    num >>= firstBit;
    uint32_t mask = 0xFFFFFF >> (16 - (lastBit - firstBit));
    return num ^= mask;
}