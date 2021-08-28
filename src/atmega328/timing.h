#pragma once
;
#include <unistd.h>
#include <time.h>


// Public
// ____________________________________________________________________________________________________________________

long tim_getTimeNanoSec() {
    struct timespec currentTime;
    clock_gettime(CLOCK_REALTIME, &currentTime);
    return currentTime.tv_nsec;
}

int tim_convertNanoSecToCycles(long nanoSeconds) {
    return nanoSeconds / (1000000000/g_atmegaClockSpeed);
}

long tim_convertCyclesToNanoSec(int cycleCount) {
    return cycleCount * (1000000000/g_atmegaClockSpeed);
}

void tim_sleepNanoSec(long nanoSeconds) {
    struct timespec requestedSleep;
    requestedSleep.tv_nsec = nanoSeconds;
    struct timespec rmtp;
    nanosleep(&requestedSleep, &rmtp);
}