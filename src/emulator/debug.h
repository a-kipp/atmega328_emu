#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "utility_functions.h"
#include "memory.h"
#include "global_variables.h"
#include "memory_map.h"


void deb_init() {
    uint8_t *r0_ptr = _dataMemory_ptr + R0;
    uint8_t *r1_ptr = _dataMemory_ptr + R1;
    uint8_t *r2_ptr = _dataMemory_ptr + R2;
    uint8_t *r3_ptr = _dataMemory_ptr + R3;
    uint8_t *r4_ptr = _dataMemory_ptr + R4;
    uint8_t *r5_ptr = _dataMemory_ptr + R5;
    uint8_t *r6_ptr = _dataMemory_ptr + R6;
    uint8_t *r7_ptr = _dataMemory_ptr + R7;
    uint8_t *r8_ptr = _dataMemory_ptr + R8;
    uint8_t *r9_ptr = _dataMemory_ptr + R9;
    uint8_t *r10_ptr = _dataMemory_ptr + R10;
    uint8_t *r11_ptr = _dataMemory_ptr + R11;
    uint8_t *r12_ptr = _dataMemory_ptr + R12;
    uint8_t *r13_ptr = _dataMemory_ptr + R13;
    uint8_t *r14_ptr = _dataMemory_ptr + R14;
    uint8_t *r15_ptr = _dataMemory_ptr + R15;
    uint8_t *r16_ptr = _dataMemory_ptr + R16;
    uint8_t *r17_ptr = _dataMemory_ptr + R17;
    uint8_t *r18_ptr = _dataMemory_ptr + R18;
    uint8_t *r19_ptr = _dataMemory_ptr + R19;
    uint8_t *r20_ptr = _dataMemory_ptr + R20;
    uint8_t *r21_ptr = _dataMemory_ptr + R21;
    uint8_t *r22_ptr = _dataMemory_ptr + R22;
    uint8_t *r23_ptr = _dataMemory_ptr + R23;
    uint8_t *r24_ptr = _dataMemory_ptr + R24;
    uint8_t *r25_ptr = _dataMemory_ptr + R25;
    uint16_t *r26r27_ptr = (uint16_t*)(_dataMemory_ptr + R26_X_REGISTER_LOW_BYTE);
    uint16_t *r28r29_ptr = (uint16_t*)(_dataMemory_ptr + R28_Y_REGISTER_LOW_BYTE);
    uint16_t *r30r31_ptr = (uint16_t*)(_dataMemory_ptr + R30_Z_REGISTER_LOW_BYTE);
}


void deb_printBinary16bit(uint16_t integer_number) {
    for(int i=0; i<16; i++) {
        if(i%8 == 0) printf(" ");
        printf("%d", (integer_number>>15));
        integer_number<<=1;
    }
    printf("\n");
}


char *deb_getName(int address) {
    switch (address) {
        case R0: return "R0";
        case R1: return "R1";
        case R2: return "R2";
        case R3: return "R3";
        case R4: return "R4";
        case R5: return "R5";
        case R6: return "R6";
        case R7: return "R7";
        case R8: return "R8";
        case R9: return "R9";
        case R10: return "R10";
        case R11: return "R11";
        case R12: return "R12";
        case R13: return "R13";
        case R14: return "R14";
        case R15: return "R15";
        case R16: return "R16";
        case R17: return "R17";
        case R18: return "R18";
        case R19: return "R19";
        case R20: return "R20";
        case R21: return "R21";
        case R22: return "R22";
        case R23: return "R23";
        case R24: return "R24";
        case R25: return "R25";
        case R26_X_REGISTER_LOW_BYTE: return "R26";
        case R27_X_REGISTER_HIGH_BYTE: return "R27";
        case R28_Y_REGISTER_LOW_BYTE: return "R28";
        case R29_Y_REGISTER_HIGH_BYTE: return "R29";
        case R30_Z_REGISTER_LOW_BYTE: return "R30";
        case R31_Z_REGISTER_HIGH_BYTE: return "R31";
        case DDRB: return "DDRB";
        case PORTB: return "PORTB"; 
        case GPIOR0: return "GPIOR0";
        case EECR: return "EECR";
        case EEDR: return "EEDR";
        case EEARL: return "EEARL";
        case EEARH: return "EEARH";
        case GPIOR1: return "GPIOR1";
        case GPIOR2: return "GPIOR2";
        case SPL: return "SPL";
        case SPH: return "SPH";
        case SREG: return "SREG";
        default: return "UNKNOWN_ADDRESS";
    }
}