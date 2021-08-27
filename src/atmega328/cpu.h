// cpu is stateless all information about the current machine state are at memory.h, that means
// the cpu thread can be stopped and started at all time, the cpu will continue execution where it
// has been stopped last time

#pragma once

#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "jump_table.h"
#include "memory.h"
#include "pin.h"
;

#define CPU_STOP 1
int _cpuSignal;

static int instructionCount = 0;

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
    uint16_t instruction = mem_programMemoryFetchInstruction(mem_programCounter);
    jump_table[instruction]();
}


static int _calculateCyclesTillNextPinChange(PinConfiguration lastPinChange, PinConfiguration nextPinChange) {

}


static void *_run(void *arg) {

    _cpuSignal = 0;
    printf("cpu started\n");
    clock_t start = clock(), diff;

    while(!_cpuSignal) {
        _executeSingleInstruction();
        instructionCount ++;
    }

    _cpuSignal = 0;
    printf("cpu stopped\n");
    diff = clock() - start;

    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Executing %d instructions took %d seconds %d milliseconds\n",instructionCount, msec/1000, msec%1000);
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