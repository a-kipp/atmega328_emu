#include <stdbool.h>
#include "../eventhandler/eventhandler.h"
#include "timing.h"
#include "../cpu/cpu.h"
;

void sch_runSnailMode() {

    while(true) {
        
        Event nextEvent = eve_getNextEvent();

        if (nextEvent.timeStamp > tim_getEmulatorTime()) {
            long long timeDelta = nextEvent.timeStamp - tim_getEmulatorTime();
            long long cycleDelta = tim_convertNanoSecToCycles(timeDelta);
            _executeInstructions(cycleDelta);
            eve_handleEvent(nextEvent);
        }





        // clock_gettime(CLOCK_REALTIME, &startTime);

        // long cycleCountStart = g_cpuCycleCount;

        // _executeInstructions(1);


        // clock_gettime(CLOCK_REALTIME, &stopTime);
        // timeTaken = _calcTimeDiff(startTime, stopTime);

        //requestedSleep.tv_nsec = (g_cpuCycleCount - cycleCountStart) * timePerCycleNanoSec - timeTaken.tv_nsec;
        //if (nanosleep(&requestedSleep, &rmtp)) {
        //    fprintf(stderr, "can't sleep\n");
        //}
    }
    _cpuStopSignal = 0;
    printf("cpu stopped\n");
}