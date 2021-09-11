#include <stdio.h>
#include <time.h>
;
#define NANOSEC_PER_SEC 1000000000

typedef struct timespec TimeObj;




// Public
// ____________________________________________________________________________________________________________________

TimeObj tim_getCurrentTime() {
    struct timespec timeStamp = {0};
    clock_gettime(CLOCK_REALTIME, &timeStamp);
    return (TimeObj)timeStamp;
}


uint64_t tim_getTimeElapsed(TimeObj timeStamp) {
    struct timespec currentTime = {0};
    clock_gettime(CLOCK_REALTIME, &currentTime);
    uint64_t elapsed = (currentTime.tv_nsec - timeStamp.tv_nsec);
    elapsed += (currentTime.tv_sec - timeStamp.tv_sec) * NANOSEC_PER_SEC;
    return elapsed;
}


void tim_sleep(long long sleepTime) {
    struct timespec rmtp = {0};
    struct timespec requestedSleep = {0};
    requestedSleep.tv_nsec = (long)sleepTime;
    if (nanosleep(&requestedSleep, &rmtp)) {
        //fprintf(stderr, "can't sleep\n");
    }
}

