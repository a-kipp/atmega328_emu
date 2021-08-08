// All functions in this file are free from side effects. No global variables are used.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


uint16_t byteswap_16bit(uint16_t num) {
    return num << 8 | num >> 8;
}

uint8_t get_bit(uint8_t* byte, int bit) {
    bit = 1 << bit;
    return(bit & *byte);
}

void set_bit(uint8_t *byte, int bit) {
    bit = 1 << bit;
    *byte = *byte | bit;
}

void clear_bit(uint8_t *byte, int bit) {
    bit = 1 << bit;
    bit ^= 0xff;
    *byte = *byte & bit;
}

uint32_t get_bits(uint32_t four_byte, int first_bit, int last_bit) {
    four_byte >>= first_bit;
    uint32_t mask = 0xFFFFFF >> (16 - (last_bit - first_bit));
    return four_byte ^= mask;
}

void load_program(uint8_t *memory, char *path_to_file, int memory_size) {
    FILE *file_ptr = fopen(path_to_file, "r");
    if(file_ptr == NULL) printf("can't open file\n");
    fread(memory, sizeof(uint16_t), memory_size, file_ptr);
    fclose(file_ptr);
}