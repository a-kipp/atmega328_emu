// cpu is stateless all information about the current machine state are at memory.h, that means
// the cpu thread can be stopped and started at all time, the cpu will continue execution where it
// has been stopped last time

#pragma once

#include <stdint.h>
#include <pthread.h>
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


static void *_run(void *arg) {

    g_elapsedCpuCycles = 0;

    _cpuStopSignal = 0;


    int timestampCpuStart = tim_getTimeNanoSec();
    int timestampLastPinChange = tim_getTimeNanoSec();
    int timestampLastInstruction = tim_getTimeNanoSec();


    while(!_cpuStopSignal) {
        while (pin_pinChangePending) {
            PinConfiguration pendingPinChange = pin_dequeuePinChange();
            int cycleDiff = tim_convertNanoSecToCycles(pendingPinChange.timestamp - timestampLastPinChange);
            _executeInstructions(cycleDiff);
            timestampLastPinChange = pendingPinChange.timestamp;
            timestampLastInstruction = tim_getTimeNanoSec();
        }
        int calculatedCpuCycles = tim_convertNanoSecToCycles(tim_getTimeNanoSec() - timestampCpuStart);
        if(g_elapsedCpuCycles <= calculatedCpuCycles) {
            _executeInstructions(calculatedCpuCycles - g_elapsedCpuCycles);
            timestampLastInstruction = tim_getTimeNanoSec();
            timestampLastPinChange = tim_getTimeNanoSec();
        } else {
            int timeDiff = tim_convertCyclesToNanoSec(g_elapsedCpuCycles - calculatedCpuCycles);
            tim_sleepNanoSec(timeDiff * 1.2);
        }
    }
    _cpuStopSignal = 0;
}



static void *_runOld(void *arg) {

    _cpuStopSignal = 0;
    printf("cpu started\n");
    clock_t start = clock(), diff;

    while(!_cpuStopSignal) {
        _executeSingleInstruction();
    }

    _cpuStopSignal = 0;
    printf("cpu stopped\n");
    diff = clock() - start;

    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Executing %d cycles took %d seconds %d milliseconds\n",g_elapsedCpuCycles, msec/1000, msec%1000);
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