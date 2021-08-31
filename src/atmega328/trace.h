#pragma once

typedef struct PointerValuePair {
    uint8_t *pointer;
    uint8_t value;
} PointerValuePair;

PointerValuePair trace[10000] = {0};

