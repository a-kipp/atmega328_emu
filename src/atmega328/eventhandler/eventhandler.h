#pragma once

typedef struct Event {
    long long timeStamp;
    uint8_t type;
} Event;
;

Event eve_getNextEvent() {
}

void eve_handleEvent() {
}