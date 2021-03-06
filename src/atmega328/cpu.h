// cpu is stateless all information about the current machine state are at memory.h, that means
// the cpu thread can be stopped and started at all time, the cpu will continue execution where it
// has been stopped last time

#pragma once

#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include "memory/memory.h"
#include "instructions/jump_table_implementation.h"
#include "instructions/instructions.h"
#include "timing.h"
#include "debug.h"
#include "eventhandler.h"
#include "declarations.h"
#include "interrupts.h"
;

bool _cpuStopSignal;


pthread_t *_cpuThread;


static void _incrementCycleCounterOnce() {
    cpu_cycleCounter ++;
    uint8_t timerCounter = mem_dataMemory[TCNT0];
    timerCounter ++;
    if (timerCounter == 0) {
        mem_dataMemory[TIFR0] = (1 << TOV0);
    }
    mem_dataMemory[TCNT0] = timerCounter;
}



void cpu_incrementCycleCounter(uint8_t increments) {
    for(uint8_t i = 0; i <= increments; i++) {
        _incrementCycleCounterOnce();
    }
}


void cpu_decrementIncrementStackPointer(int8_t increments) {
    mem_dataMemory[STACKPOINTER] += increments;
}






static void *_run(void *arg) {

    uint64_t calcExecutionTime = 0;
    uint64_t actualExecutionTime = 0;
    uint64_t totalTimeTaken = 0;
    uint64_t delta = 0;
    uint16_t instructionsToExecude = (cpu_clockSpeed / 1000000) + 1;

    _cpuStopSignal = false;

    while(!_cpuStopSignal) {
        TimeObj timeStamp = tim_getCurrentTime();

        eve_handleEvents();

        long long cycleCountStart = cpu_cycleCounter;

        for (int i = 0; i < instructionsToExecude; i++) {

            uint16_t opCode = mem_fetchInstruction(reg_programCounter);

            jti_implementationTable[opCode]();

            int_handleInterrupts();
        }

        calcExecutionTime = (cpu_cycleCounter - cycleCountStart) * (NANOSEC_PER_SEC / cpu_clockSpeed);

        actualExecutionTime = tim_getTimeElapsed(timeStamp);

        tim_sleep(calcExecutionTime - actualExecutionTime - delta);

        totalTimeTaken = tim_getTimeElapsed(timeStamp) + delta;

        delta = totalTimeTaken - calcExecutionTime;
    }
    _cpuStopSignal = false;
    printf("cpu stopped\n");
}





static void *_runVerbose(void *arg) {

    uint64_t calcExecutionTime = 0;
    uint64_t actualExecutionTime = 0;
    uint64_t totalTimeTaken = 0;
    uint64_t delta = 0;
    uint16_t instructionsToExecude = (cpu_clockSpeed / 1000000) + 1;

    _cpuStopSignal = false;

    while(!_cpuStopSignal) {
        TimeObj timeStamp = tim_getCurrentTime();

        printf("%04X ",  reg_programCounter);
        uti_print_binary_8bit(mem_dataRead8bit(SREG));
        printf(" %s",  ins_disassembleInstruction(reg_programCounter).info);
        printf("     %s\n",  ins_disassembleInstruction(reg_programCounter).comment);

        eve_handleEvents();

        long long cycleCountStart = cpu_cycleCounter;

        for (int i = 0; i < instructionsToExecude; i++) {

            uint16_t opCode = mem_fetchInstruction(reg_programCounter);

            jti_implementationTable[opCode]();

            int_handleInterrupts();
        }

        calcExecutionTime = (cpu_cycleCounter - cycleCountStart) * (NANOSEC_PER_SEC / cpu_clockSpeed);

        actualExecutionTime = tim_getTimeElapsed(timeStamp);

        tim_sleep(calcExecutionTime - actualExecutionTime - delta);

        totalTimeTaken = tim_getTimeElapsed(timeStamp) + delta;

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


void cpu_startVerbose() {
    pthread_t newThread;
    _cpuThread = &newThread;
    pthread_create(_cpuThread, NULL, _runVerbose, NULL);
}


void cpu_stop() {
    printf("cpu stop called\n");
    _cpuStopSignal = true;
}