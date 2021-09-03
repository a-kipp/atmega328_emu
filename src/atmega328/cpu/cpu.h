// cpu is stateless all information about the current machine state are at memory.h, that means
// the cpu thread can be stopped and started at all time, the cpu will continue execution where it
// has been stopped last time

#pragma once

#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include "../memory/memory.h"
#include "../instructions/jump_table_implementation.h"
#include "../instructions/instructions.h"
#include "../config.h"
#include "../timing.h"
#include "../debug.h"
;

int _cpuStopSignal;


pthread_t *_cpuThread;


static void *z_run(void *arg) {

    mem_cpuCycleCount = 0;
    _cpuStopSignal = 0;

    struct timespec timeStamp = {0};


    while(!_cpuStopSignal) {

        
        uint64_t cycleTime = getTimeDelta(tim_getRealTime(), timeStamp);

        timeStamp = tim_getRealTime();

        for (int i = 0; i < 1000; i++) {
            uint16_t opCode = mem_fetchInstruction(mem_programCounter);
            jti_implementationTable[opCode]();
        }

        uint64_t offset = (NANOSEC_PER_SEC /  g_clockSpeed) * 1000 - cycleTime;

        uint64_t delta = getTimeDelta(tim_getRealTime(), timeStamp);

        tim_sleepNanoSec((NANOSEC_PER_SEC /  g_clockSpeed) * 600 - delta + offset);
    }
    _cpuStopSignal = 0;
    printf("cpu stopped\n");
}


static void *x_run(void *arg) {

    mem_cpuCycleCount = 0;
    _cpuStopSignal = 0;

    struct timespec timeStamp = {0};


    while(!_cpuStopSignal) {

        uint16_t opCode = mem_fetchInstruction(mem_programCounter);
        jti_implementationTable[opCode]();
    }
    _cpuStopSignal = 0;
    printf("cpu stopped\n");
}


static void *_run(void *arg) {

    mem_cpuCycleCount = 0;
    _cpuStopSignal = 0;

    struct timespec timeStamp = {0};


    while(!_cpuStopSignal) {

        printf("%04X ",  mem_programCounter);
        uti_print_binary_8bit(mem_dataRead8bit(SREG));
        printf(" %s\n",  ins_info(mem_fetchInstruction(mem_programCounter)).info);

        
        uint64_t cycleTime = getTimeDelta(tim_getRealTime(), timeStamp);

        timeStamp = tim_getRealTime();

        for (int i = 0; i < 1; i++) {
            uint16_t opCode = mem_fetchInstruction(mem_programCounter);
            jti_implementationTable[opCode]();
        }

        uint64_t offset = (NANOSEC_PER_SEC /  g_clockSpeed) - cycleTime;

        uint64_t delta = getTimeDelta(tim_getRealTime(), timeStamp);


        tim_sleepNanoSec((NANOSEC_PER_SEC /  g_clockSpeed)  - delta);
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