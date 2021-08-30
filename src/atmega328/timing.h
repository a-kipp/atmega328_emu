#include <stdio.h>
#include <time.h>
#include "global_variables.h"


static unsigned long long _cycleCountStart = 0;
static bool _speedTestIsRunning = false;


static struct timespec _startTime;
static struct timespec _timeTaken;
static struct timespec _stopTime;


// Public
// ____________________________________________________________________________________________________________________

void tim_triggerSpeedTest() {
    if (!_speedTestIsRunning) {
        _cycleCountStart = g_cpuCycleCount;
        clock_gettime(CLOCK_REALTIME, &_startTime);

        _speedTestIsRunning = true;
        printf("speedtest start\n");
    } else {
        clock_gettime(CLOCK_REALTIME, &_stopTime);

        _speedTestIsRunning = false;

        _timeTaken.tv_sec = _stopTime.tv_sec - _startTime.tv_sec;
        _timeTaken.tv_nsec = _stopTime.tv_nsec - _startTime.tv_nsec;

        double elapsed = (double)_stopTime.tv_sec - (double)_startTime.tv_sec;
        elapsed += ((double)_stopTime.tv_nsec / 1000000000);
        elapsed -= ((double)_startTime.tv_nsec / 1000000000);

        double speedHz = (double)(g_cpuCycleCount - _cycleCountStart) / elapsed;
    
        printf("cycles elapsed: %d\n", g_cpuCycleCount - _cycleCountStart);
        printf("time elapsed: %f\n", elapsed);
        printf("speed: %f Hz\n", speedHz);
    }
}
