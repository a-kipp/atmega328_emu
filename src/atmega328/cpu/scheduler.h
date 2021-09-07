#include <stdbool.h>
#include "../eventhandler/eventhandler.h"
#include "timing.h"
#include "../cpu/cpu.h"
;

void sch_runFastMode() {

    while(true) {
        long long timeDelta;
        long long cycleDelta;
        
        Event nextEvent = eve_getNextEvent();

        if (nextEvent.timeStamp > tim_getEmulatorTime()) {
            timeDelta = nextEvent.timeStamp - tim_getEmulatorTime();
            cycleDelta = tim_convertNanoSecToCycles(timeDelta);
            _executeInstructions(cycleDelta);
            eve_handleEvent(nextEvent);
        } else {
            tim_sleep(timeDelta);
        }
    }
}