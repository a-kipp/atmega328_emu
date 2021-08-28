// cpu is stateless all information about the current machine state are at memory.h, that means
// the cpu thread can be stopped and started at all time, the cpu will continue execution where it
// has been stopped last time

#pragma once

#include <unistd.h>
#include <time.h>

#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "jump_table.h"
#include "../timing.h"
#include "../memory/memory.h"
#include "../pin.h"
;

#define CPU_STOP 1
int _cpuStopSignal;


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
    printf("time1 %d %d \n", timeStamp1.tv_sec, timeStamp1.tv_nsec);
    printf("time2 %d %d \n", timeStamp2.tv_sec, timeStamp2.tv_nsec);
    printf("diff %d %d \n", diff.tv_sec, diff.tv_nsec);
    return diff;
}


static void *_run(void *arg) {

    if (g_atmegaClockSpeed <= 2) {
        fprintf(stderr, "clock speed to slow\n");
        exit(1);
    }

    g_cpuCycleCount = 0;
    _cpuStopSignal = 0;

    unsigned int instructionsToExecude = g_atmegaClockSpeed / 160000 + 1;
    long timePerCycleNanoSec = 1000000000 / g_atmegaClockSpeed;

    struct timespec requestedSleep;
    struct timespec rmtp;
    struct timespec currentTime;

    //clock_gettime(CLOCK_REALTIME, &currentTime);

    struct timespec startTime = currentTime;
    struct timespec timeTaken = currentTime;
    struct timespec offsetTime = currentTime;
    struct timespec stopTime = currentTime;


    while(!_cpuStopSignal) {
        clock_gettime(CLOCK_REALTIME, &startTime);
        long cycleCountStart = g_cpuCycleCount;
        _executeInstructions(instructionsToExecude);
        clock_gettime(CLOCK_REALTIME, &stopTime);

        timeTaken = _calcTimeDiff(startTime, stopTime);

        requestedSleep.tv_nsec = (g_cpuCycleCount - cycleCountStart) * timePerCycleNanoSec - timeTaken.tv_nsec;

        if (!nanosleep(&requestedSleep, &rmtp)) {
            printf("habe gut geschlafen\n");
        } else {
            printf("habe schlecht geschlafen\n");
        };

        printf("elapsed cpu cycles %d \n", g_cpuCycleCount - cycleCountStart);
        printf("instruction to execude %d \n", instructionsToExecude);
        printf("gehe schlafen für %d Nanosekunden \n", requestedSleep.tv_nsec);
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
    _cpuStopSignal = CPU_STOP;
}