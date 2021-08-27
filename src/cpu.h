#pragma once

#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "jump_table.h"
#include "memory.h"
;

#define CPU_STOP 1
int _cpuSignal;

pthread_t *_cpuThread;


void _execute_single_instruction() {
    uint16_t instruction = mem_programMemoryFetchInstruction(mem_programCounter);
    jump_table[instruction]();
}


void *_run(void *arg) {
    int instructionCount = 0;

    _cpuSignal = 0;
    printf("cpu started\n");
    clock_t start = clock(), diff;

    while(!_cpuSignal) {
        _execute_single_instruction();
        instructionCount ++;
    }

    _cpuSignal = 0;
    printf("cpu stopped\n");
    diff = clock() - start;

    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Executing %d instructions took %d seconds %d milliseconds\n",instructionCount, msec/1000, msec%1000);
}


void cpu_start() {
    pthread_t newThread;
    _cpuThread = &newThread;
    pthread_create(_cpuThread, NULL, _run, NULL);
}


void cpu_stop() {
    printf("cpu stop called\n");
    _cpuSignal = CPU_STOP;
}