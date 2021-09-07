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
#include "../globalconfig.h"
#include "../timing.h"
#include "../debug.h"
;

bool _cpuStopSignal;


pthread_t *_cpuThread;


static void *_run(void *arg) {

    uint64_t calcExecutionTime = 0;
    uint64_t actualExecutionTime = 0;
    uint64_t totalTimeTaken = 0;
    uint64_t delta = 0;
    uint16_t instructionsToExecude = (g_clockSpeed / 1000000) + 1;

    _cpuStopSignal = false;

    while(!_cpuStopSignal) {
        tim_timeStart();

        //printf("%04X ",  mem_programCounter);
        //uti_print_binary_8bit(mem_dataRead8bit(SREG));
        //printf(" %s",  ins_disassembleInstruction(mem_programCounter).info);
        //printf("     %s\n",  ins_disassembleInstruction(mem_programCounter).comment);

        long long cycleCountStart = mem_cpuCycleCount;

        for (int i = 0; i < instructionsToExecude; i++) {
            uint16_t opCode = mem_fetchInstruction(mem_programCounter);
            jti_implementationTable[opCode]();
        }

        calcExecutionTime = (mem_cpuCycleCount - cycleCountStart) * (NANOSEC_PER_SEC / g_clockSpeed);
        actualExecutionTime = tim_getTimeElapsed();

        tim_sleep(calcExecutionTime - actualExecutionTime - delta);
        totalTimeTaken = tim_getTimeElapsed() + delta;
        delta = totalTimeTaken - calcExecutionTime;
    }

    _cpuStopSignal = false;
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