#pragma once

#include <unistd.h>
#include <time.h>


int tim_getTime() {
    struct timespec time;
    clock_gettime( CLOCK_REALTIME, &time);
    return (int)time.tv_nsec;
}