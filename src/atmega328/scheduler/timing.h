#include <stdio.h>
#include <time.h>

#define NANOSEC_PER_SEC 1000000000

static long long _emulationStartTime;
static bool _startTimeIsSet;


static struct timespec _stopedTime;



long long _convertTimespecToLonLong(struct timespec time) {
    long long convertedTime;
    convertedTime = time.tv_sec;
    convertedTime += time.tv_nsec;
}



// Public
// ____________________________________________________________________________________________________________________




void tim_setTimeEmulationStart() {
    static bool timeIsSet;
    if (_startTimeIsSet) {
        fprintf(stderr, "tried to set emulationStartTime twice\n");
    } else {
        _emulationStartTime = (g_cpuCycleCount / tim_atmegaClockSpeed) * NANOSEC_PER_SEC;
    }
}


long long tim_getEmulatorTime() {
    return (g_cpuCycleCount / tim_atmegaClockSpeed) * NANOSEC_PER_SEC;
}


long long tim_getRealTime() {
    clock_gettime(CLOCK_REALTIME, &_stopedTime);
    long long currentTime = _convertTimespecToLonLong(_stopedTime);
    return currentTime - _emulationStartTime;
}


long long tim_convertNanoSecToCycles(long long time) { // ago I can still remember...
    long long cycles;
    cycles = (time / (NANOSEC_PER_SEC * tim_atmegaClockSpeed));
    return cycles;
}