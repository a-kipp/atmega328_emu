#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "utility_functions.h"
#include "memory/memory.h"
#include "global_variables.h"
#include "memory/map.h"
#include <time.h>

// Public
// ____________________________________________________________________________________________________________________

//void deb_init() {
//    uint8_t *r0_ptr = vir_dataMemory_ptr + R0;
//    uint8_t *r1_ptr = vir_dataMemory_ptr + R1;
//    uint8_t *r2_ptr = vir_dataMemory_ptr + R2;
//    uint8_t *r3_ptr = vir_dataMemory_ptr + R3;
//    uint8_t *r4_ptr = vir_dataMemory_ptr + R4;
//    uint8_t *r5_ptr = vir_dataMemory_ptr + R5;
//    uint8_t *r6_ptr = vir_dataMemory_ptr + R6;
//    uint8_t *r7_ptr = vir_dataMemory_ptr + R7;
//    uint8_t *r8_ptr = vir_dataMemory_ptr + R8;
//    uint8_t *r9_ptr = vir_dataMemory_ptr + R9;
//    uint8_t *r10_ptr = vir_dataMemory_ptr + R10;
//    uint8_t *r11_ptr = vir_dataMemory_ptr + R11;
//    uint8_t *r12_ptr = vir_dataMemory_ptr + R12;
//    uint8_t *r13_ptr = vir_dataMemory_ptr + R13;
//    uint8_t *r14_ptr = vir_dataMemory_ptr + R14;
//    uint8_t *r15_ptr = vir_dataMemory_ptr + R15;
//    uint8_t *r16_ptr = vir_dataMemory_ptr + R16;
//    uint8_t *r17_ptr = vir_dataMemory_ptr + R17;
//    uint8_t *r18_ptr = vir_dataMemory_ptr + R18;
//    uint8_t *r19_ptr = vir_dataMemory_ptr + R19;
//    uint8_t *r20_ptr = vir_dataMemory_ptr + R20;
//    uint8_t *r21_ptr = vir_dataMemory_ptr + R21;
//    uint8_t *r22_ptr = vir_dataMemory_ptr + R22;
//    uint8_t *r23_ptr = vir_dataMemory_ptr + R23;
//    uint8_t *r24_ptr = vir_dataMemory_ptr + R24;
//    uint8_t *r25_ptr = vir_dataMemory_ptr + R25;
//    uint16_t *r26r27_ptr = (uint16_t*)(vir_dataMemory_ptr + R26_X_REGISTER_LOW_BYTE);
//    uint16_t *r28r29_ptr = (uint16_t*)(vir_dataMemory_ptr + R28_Y_REGISTER_LOW_BYTE);
//    uint16_t *r30r31_ptr = (uint16_t*)(vir_dataMemory_ptr + R30_Z_REGISTER_LOW_BYTE);
//}


void deb_printBinary16bit(uint16_t integer_number) {
    for(int i=0; i<16; i++) {
        printf("%d", (integer_number>>15));
        integer_number<<=1;
    }
}

void deb_print_binary_8bit(uint8_t integer_number) {
    for(int i=0; i<8; i++) {
        printf("%d", (integer_number>>7));
        integer_number<<=1;
    }
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





static unsigned long long _cycleCountStart = 0;
static bool _speedTestIsRunning = false;


static struct timespec _startTime;
static struct timespec _timeTaken;
static struct timespec _stopTime;

static struct timespec _stopedTime;


// Public
// ____________________________________________________________________________________________________________________

unsigned int tim_atmegaClockSpeed = 16000000; //minimum 2 Hz


void deb_triggerSpeedTest() {
    if (!_speedTestIsRunning) {
        _cycleCountStart = g_cpuCycleCount;
        clock_gettime(CLOCK_REALTIME, &_startTime);

        _speedTestIsRunning = true;
        printf("speedtest start\n");
    } else {
        clock_gettime(CLOCK_REALTIME, &_stopTime);

        _speedTestIsRunning = false;

        _timeTaken.tv_sec = _stopTime.tv_sec - _startTime.tv_sec;
        _timeTaken.tv_nsec = _stopTime.tv_nsec - _startTime.tv_nsec;

        double elapsed = (double)_stopTime.tv_sec - (double)_startTime.tv_sec;
        elapsed += ((double)_stopTime.tv_nsec / 1000000000);
        elapsed -= ((double)_startTime.tv_nsec / 1000000000);

        double speedHz = (double)(g_cpuCycleCount - _cycleCountStart) / elapsed;
    
        printf("cycles elapsed: %d\n", g_cpuCycleCount - _cycleCountStart);
        printf("time elapsed: %f\n", elapsed);
        printf("speed: %f Hz\n", speedHz);
    }
}
