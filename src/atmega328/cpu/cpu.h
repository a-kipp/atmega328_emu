// cpu is stateless all information about the current machine state are at memory.h, that means
// the cpu thread can be stopped and started at all time, the cpu will continue execution where it
// has been stopped last time

#pragma once

#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "../memory/memory.h"
#include "instructions/jump_table_implementation.h"
#include "instructions/instruction.h"
;

int _cpuStopSignal;


pthread_t *_cpuThread;



static void _executeInstructions(int numberOfInstructions) {
    for (int i = 0; i < numberOfInstructions; i++) {
        uint16_t opCode = mem_programFetchInstruction(mem_programCounter);
        //deb_print_binary_8bit(arr_dataMemory[SREG]);
        //printf("\n");
        //printf("%04X ", mem_programCounter);
        //printf("%s\n", jtd_disassembleTable[opCode](opCode).info);
        jti_implementationTable[opCode]();
    }
}

struct timespec _calcTimeDiff(struct timespec timeStamp1, struct timespec timeStamp2) {
    struct timespec diff;
    diff.tv_sec = 0;
    diff.tv_nsec = (timeStamp2.tv_nsec - timeStamp1.tv_nsec) + (timeStamp2.tv_sec - timeStamp1.tv_sec) * 1000000000;
    return diff;
}


static void *_run(void *arg) {

    if (tim_atmegaClockSpeed < 2) {
        fprintf(stderr, "clock speed to slow\n");
        exit(1);
    }

    g_cpuCycleCount = 0;
    _cpuStopSignal = 0;

    unsigned int instructionsToExecude = tim_atmegaClockSpeed / 160000 + 1;
    long timePerCycleNanoSec = 1000000000 / tim_atmegaClockSpeed;

    struct timespec requestedSleep;
    struct timespec rmtp;
    struct timespec currentTime;


    struct timespec startTime = currentTime;
    struct timespec timeTaken = currentTime;
    struct timespec stopTime = currentTime;


    while(!_cpuStopSignal) {
        clock_gettime(CLOCK_REALTIME, &startTime);

        long cycleCountStart = g_cpuCycleCount;

        _executeInstructions(1);


        clock_gettime(CLOCK_REALTIME, &stopTime);
        timeTaken = _calcTimeDiff(startTime, stopTime);

        //requestedSleep.tv_nsec = (g_cpuCycleCount - cycleCountStart) * timePerCycleNanoSec - timeTaken.tv_nsec;
        //if (nanosleep(&requestedSleep, &rmtp)) {
        //    fprintf(stderr, "can't sleep\n");
        //}
    }
    _cpuStopSignal = 0;
    printf("cpu stopped\n");
}




// Public
// ____________________________________________________________________________________________________________________

void cpu_start() {
    pthread_t newThread;
    _cpuThread = &newThread;
    pthread_create(_cpuThread, NULL, _run, NULL);
}


void cpu_stop() {
    printf("cpu stop called\n");
    _cpuStopSignal = true;
}