#pragma once


// extract addresses and immediate values from an instruction
uint16_t dec_extractBits0000000111110000(uint16_t instruction) {
    return (instruction & 0b0000000111110000) >> 4;
}

uint16_t dec_extractBits0000000011110000(uint16_t instruction) {
    return (instruction & 0b0000000011110000) >> 4;
}

uint16_t dec_extractBits0000111100001111(uint16_t instruction) {
    return (instruction & 0b0000111100000000) >> 4 | (instruction & 0b0000000000001111);
}

uint16_t dec_extractBits0000011000001111(uint16_t instruction) {
    return (instruction & 0b0000011000000000) >> 5 | (instruction & 0b0000000000001111);
}

uint16_t dec_extractBits0000001000001111(uint16_t instruction) {
    return (instruction & 0b0000001000000000) >> 5 | (instruction & 0b0000000000001111);
}

uint16_t dec_extractBits0000000000110000(uint16_t instruction) {
    return (instruction & 0b0000000000110000) >> 4;
}

uint16_t dec_extractBits0000000011001111(uint16_t instruction) {
    return (instruction & 0b0000000011000000) >> 6 | (instruction & 0b0000000000001111);
}

uint16_t dec_extractBits0000001111111000(uint16_t instruction) {
    return (instruction & 0b0000001111111000) >> 3;
}

uint16_t dec_extractBits0000111111111111(uint16_t instruction) {
    return (instruction & 0b0000111111111111);
}

uint16_t dec_extractBits0000000001111000(uint16_t instruction) {
    return (instruction & 0b0000000001111000) >> 3;
}

uint16_t dec_extractBits0000000000000111(uint16_t instruction) {
    return (instruction & 0b0000000000000111);
}

uint16_t dec_extractBits0000011111111111(uint16_t instruction) {
    return (instruction & 0b0000011111111111);
}