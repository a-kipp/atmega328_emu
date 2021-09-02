#include <stdio.h>
#include <time.h>
#include "config.h"

#define NANOSEC_PER_SEC 1000000000

static long long _emulationStartTime;
static bool _startTimeIsSet;


static struct timespec _stopedTime;
static struct timespec _requestedSleep;
static struct timespec _rmtp;


long long _convertTimespecToLonLong(struct timespec time) {
    long long convertedTime;
    convertedTime = (time.tv_sec - 1000000);// (long long)NANOSEC_PER_SEC;
    //convertedTime += time.tv_nsec;
    printf("seconds: %d \n", time.tv_sec);
    printf("nano seconds %d \n", time.tv_nsec);
    printf("convertedTime time: %d \n", convertedTime);
}



// Public
// ____________________________________________________________________________________________________________________




void tim_setTimeEmulationStart() {
    static bool timeIsSet;
    if (_startTimeIsSet) {
        fprintf(stderr, "tried to set emulationStartTime twice\n");
    } else {
        _emulationStartTime = (mem_cpuCycleCount / g_clockSpeed) * NANOSEC_PER_SEC;
    }
}


long long tim_getEmulatorTime() {
    return (mem_cpuCycleCount / g_clockSpeed) * NANOSEC_PER_SEC;
}


struct timespec tim_getRealTime() {
    clock_gettime(CLOCK_REALTIME, &_stopedTime);
    struct timespec currentTime;
    currentTime = _stopedTime;
    return currentTime;
}


long long tim_convertNanoSecToCycles(long long time) { // ago I can still remember...
    long long cycles;
    cycles = (time / (NANOSEC_PER_SEC * g_clockSpeed));
    return cycles;
}


void tim_sleepNanoSec(long long sleepTime) {
    _requestedSleep.tv_nsec = (long)sleepTime;
    if (nanosleep(&_requestedSleep, &_rmtp)) {
        //fprintf(stderr, "can't sleep\n");
    }
}

uint64_t getTimeDelta(struct timespec timestamp1, struct timespec timestamp2) {
    uint64_t delta = (timestamp1.tv_sec - timestamp2.tv_sec) * NANOSEC_PER_SEC + (timestamp1.tv_nsec - timestamp2.tv_nsec);
    return delta;
}
