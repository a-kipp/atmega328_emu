#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "utility_functions.h"
#include "memory_map.h"
#include "global_variables.h"


int main(int argc, char *argv[] ) {
    if(argc != 2) {
        fprintf(stderr, "only one comandline argument allowed\n");
        exit(1);
    }

    program_memory_ptr = calloc(PROGRAM_MEMORY_END + 1, sizeof(uint8_t));
    data_memory_ptr = calloc(DATA_MEMORY_END + 1, sizeof(uint8_t));
    eeprom_memory_ptr = calloc(EEPROM_END + 1, sizeof(uint8_t));

    load_program(program_memory_ptr, argv[1], PROGRAM_MEMORY_END);
    printf("%s\n", argv[1]);
    printf("%s\n", program_memory_ptr);
}