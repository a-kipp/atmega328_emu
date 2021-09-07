#include <stdio.h>
#include <time.h>
#include "globalconfig.h"

#define NANOSEC_PER_SEC 1000000000


struct timespec _timeStampStart;
struct timespec _timeStamp;
struct timespec _requestedSleep;
struct timespec _rmtp;



// Public
// ____________________________________________________________________________________________________________________

void tim_timeStart() {
    clock_gettime(CLOCK_REALTIME, &_timeStampStart);
}


uint64_t tim_getTimeElapsed() {
    clock_gettime(CLOCK_REALTIME, &_timeStamp);
    uint64_t elapsed = (_timeStamp.tv_nsec - _timeStampStart.tv_nsec);
    elapsed += (_timeStamp.tv_sec - _timeStampStart.tv_sec) * NANOSEC_PER_SEC;
    return elapsed;
}


void tim_sleep(long long sleepTime) {
    _requestedSleep.tv_nsec = (long)sleepTime;
    if (nanosleep(&_requestedSleep, &_rmtp)) {
        //fprintf(stderr, "can't sleep\n");
    }
}

