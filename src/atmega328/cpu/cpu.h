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
#include "jump_table.h"
#include "../memory/memory.h"
#include "../pin.h"
;

#define CPU_STOP 0
#define PIN_CHANGE 1

int _cpuSignal;


pthread_t *_cpuThread;


static void _executeSingleInstruction() {
    uint16_t instruction = mem_programMemoryFetchInstruction(mem_programCounter);
    jump_table[instruction]();
}


static void _executeInstructions(int numberOfInstructions) {
    for (int i = 0; i < numberOfInstructions; i++) {
        uint16_t instruction = mem_programMemoryFetchInstruction(mem_programCounter);
        jump_table[instruction]();
    }
}

struct timespec _calcTimeDiff(struct timespec timeStamp1, struct timespec timeStamp2) {
    struct timespec diff;
    diff.tv_sec = 0;
    diff.tv_nsec = (timeStamp2.tv_nsec - timeStamp1.tv_nsec) + (timeStamp2.tv_sec - timeStamp1.tv_sec) * 1000000000;
    return diff;
}


static void *_run(void *arg) {

    if (g_atmegaClockSpeed < 2) {
        fprintf(stderr, "clock speed to slow\n");
        exit(1);
    }

    g_cpuCycleCount = 0;
    _cpuSignal = 0;

    unsigned int instructionsToExecude = g_atmegaClockSpeed / 1600000 + 1;
    long timePerCycleNanoSec = 1000000000 / g_atmegaClockSpeed;

    struct timespec requestedSleep;
    struct timespec rmtp;
    struct timespec currentTime;


    struct timespec startTime = currentTime;
    struct timespec timeTaken = currentTime;
    struct timespec stopTime = currentTime;


    while(!_cpuSignal) {
        clock_gettime(CLOCK_REALTIME, &startTime);

        if (_cpuSignal) {
            switch (_cpuSignal) {
                case PIN_CHANGE:; break;
                default: break;
            }
        }

        long cycleCountStart = g_cpuCycleCount;

        _executeInstructions(instructionsToExecude);

        clock_gettime(CLOCK_REALTIME, &stopTime);
        timeTaken = _calcTimeDiff(startTime, stopTime);

        requestedSleep.tv_nsec = (g_cpuCycleCount - cycleCountStart) * timePerCycleNanoSec - timeTaken.tv_nsec;
        if (nanosleep(&requestedSleep, &rmtp)) {
            fprintf(stderr, "can't sleep\n");
        }
    }
    _cpuSignal = 0;
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
    _cpuSignal = CPU_STOP;
}