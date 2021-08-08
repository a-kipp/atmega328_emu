#include <stdint.h>
#include <stdlib.h>
#include "memory_map.h"
#include "global_variables.h"
#include "utility_functions.h"

uint8_t *_program_mem_ptr;
uint8_t *_data_mem_ptr;
uint8_t *_eeprom_mem_ptr;



void memory_init(char* bin_file_path) {
    _program_mem_ptr = calloc(PROGRAM_MEMORY_END + 1, sizeof(uint8_t));
    _data_mem_ptr = calloc(DATA_MEMORY_END + 1, sizeof(uint8_t));
    _eeprom_mem_ptr = calloc(EEPROM_END + 1, sizeof(uint8_t));

    load_program(_program_mem_ptr, bin_file_path, PROGRAM_MEMORY_END);
    printf("%s\n", bin_file_path);
    printf("%s\n", _program_mem_ptr);
}


// Read and Write
// ____________________________________________________________________________________________________________________

uint8_t memory_program_read_8bit(uint16_t address) {
    return *(uint8_t*)(_program_mem_ptr + address);
}

uint8_t memory_data_read_8bit(uint16_t address) {
    return *(uint8_t*)(_data_mem_ptr + address);
}

uint8_t memory_eeprom_read_8bit(uint16_t address) {
    return *(uint8_t*)(_eeprom_mem_ptr + address);
}

uint16_t memory_program_read_16bit(uint16_t address) {
    uint16_t instruction = *(uint16_t*)(_program_mem_ptr + address);
    return byteswap_16bit(instruction);
}

uint16_t memory_data_read_16bit(uint16_t address) {
    uint16_t instruction = *(uint16_t*)(_data_mem_ptr + address);
    return byteswap_16bit(instruction);
}

uint16_t memory_eeprom_read_16bit(uint16_t address) {
    uint16_t instruction = *(uint16_t*)(_eeprom_mem_ptr + address);
    return byteswap_16bit(instruction);
}

void memory_program_write_8bit(uint16_t address, uint8_t value) {
    *(uint8_t*)(_program_mem_ptr + address) = value;
}

void memory_data_write_8bit(uint16_t address, uint8_t value) {
    *(uint8_t*)(_data_mem_ptr + address) = value;
}

void memory_eeprom_write_8bit(uint16_t address, uint8_t value) {
    *(uint8_t*)(_eeprom_mem_ptr + address) = value;
}

void memory_program_write_16bit(uint16_t address, uint16_t value) {
    *(uint16_t*)(_program_mem_ptr + address) = byteswap_16bit(value);
}

void memory_data_write_16bit(uint16_t address, uint16_t value) {
    *(uint16_t*)(_data_mem_ptr + address) = byteswap_16bit(value);
}

void memory_eeprom_write_16bit(uint16_t address, uint16_t value) {
    *(uint16_t*)(_eeprom_mem_ptr + address) = byteswap_16bit(value);
}

void memory_get_sreg_flag(int flag) {
    get_bit(_data_mem_ptr + SREG, flag);
}

void memory_set_sreg_flag(int flag) {
    set_bit(_data_mem_ptr + SREG, flag);
}

void memory_clear_sreg_flag(int flag) {
    clear_bit(_data_mem_ptr + SREG, flag);
}