#include <stdint.h>
#include "memory.h"
#include "global_variables.h"
#include "utility_functions.h"

#define INSTRUCTION_DEBUG 


// Extraction Patterns
#define EXTRACTION_PATTERN_0000000111110000 0
#define EXTRACTION_PATTERN_0000000011110000 1
#define EXTRACTION_PATTERN_0000111100001111 2
#define EXTRACTION_PATTERN_0000011000001111 3
#define EXTRACTION_PATTERN_0000001000001111 4


// extracts addresses and immediate values from an instruction
uint16_t _extract_bits(uint16_t instruction, int extraction_pattern) {
    switch (extraction_pattern) {
    case EXTRACTION_PATTERN_0000000111110000:
        return (instruction & 0b0000000111110000) >> 4; break;

    case EXTRACTION_PATTERN_0000000011110000:
        return (instruction & 0b0000000011110000) >> 4; break;

    case EXTRACTION_PATTERN_0000111100001111:
        return (instruction & 0b0000111100000000) >> 4 | (instruction & 0b0000000000001111); break;

    case EXTRACTION_PATTERN_0000011000001111:
        return (instruction & 0b0000011000000000) >> 5 | (instruction & 0b0000000000001111); break;

    case EXTRACTION_PATTERN_0000001000001111:
        return (instruction & 0b0000001000000000) >> 5 | (instruction & 0b0000000000001111); break;    

    default:
        break;
    }
}



// Instruction Implementations
// ____________________________________________________________________________________________________________________


// NOP - No Operation
// This instruction performs a single cycle No Operation.
// AVR Instruction Manual page 131
void nop() {
    global_program_counter += 2;
}


// ADC - Add with Carry
// Adds two registers and the contents of the C Flag and places the result in the destination register Rd.
void adc(uint8_t rd_addr, uint8_t rr_addr) {
    uint8_t rd = *(_data_mem_ptr + rd_addr);
    uint8_t rr = *(_data_mem_ptr + rr_addr);
    uint8_t result = rd + rr + memory_get_sreg_flag(CARRY_FLAG);

    if (get_bit(rd, 3) && get_bit(rr, 3) || get_bit(rr, 3) && !get_bit(result, 3) || !get_bit(result, 3) && get_bit(rd, 3)) {
        memory_set_sreg_flag(CARRY_FLAG);
    } else {
        memory_clear_sreg_flag(CARRY_FLAG);
    }
    if (memory_get_sreg_flag(NEGATIVE_FLAG) ^ memory_get_sreg_flag(TWOS_COMPLEMENT_OVERFLOW_FLAG)) {
        memory_set_sreg_flag(SIGN_BIT);
    } else {
        memory_clear_sreg_flag(SIGN_BIT);
    }
    if (get_bit(rd, 7) && get_bit(rr, 7) && !get_bit(result, 7) || !get_bit(rd, 7) && !get_bit(rr, 7) && get_bit(result, 7)) {
        memory_set_sreg_flag(TWOS_COMPLEMENT_OVERFLOW_FLAG);
    } else {
        memory_clear_sreg_flag(TWOS_COMPLEMENT_OVERFLOW_FLAG)
    }
    if (get_bit(result, 7)) {
        memory_set_sreg_flag(NEGATIVE_FLAG);
    } else {
        memory_clear_sreg_flag(NEGATIVE_FLAG);
    }
    if (result == 0) {
        memory_set_sreg_flag(ZERO_FLAG);
    } else {
        memory_clear_sreg_flag(ZERO_FLAG);
    }
    if (get_bit(rd, 7) && get_bit(rr, 7) || get_bit(rr, 7) && get_bit(result, 7) || !get_bit(result, 7) && !get_bit(result, 7)) {
        memory_set_sreg_flag(CARRY_FLAG);
    } else {
        memory_clear_sreg_flag(CARRY_FLAG);
    }
    
    *(_data_mem_ptr + rd_addr) = result;
    global_program_counter += 2;
    INSTRUCTION_DEBUG
}


// ADD - Add without Carry
// Adds two registers without the C Flag and places the result in the destination register Rd.
void add() {

}


// CLR – Clear Register
// Clears a register. This instruction performs an Exclusive OR between a register and itself. This will clear all bits in the register.
// AVR Instruction Manual page 71
void clr() {
    uint16_t instruction = fetch_16bit_instruction(global_program_counter);
    uint16_t rd_addr = _extract_bits(instruction, EXTRACTION_PATTERN_0000000111110000);
    *(uint8_t*)(_data_mem_ptr + rd_addr) = 0;
    memory_clear_sreg_flag(SIGN_BIT);
    memory_clear_sreg_flag(TWOS_COMPLEMENT_OVERFLOW_FLAG);
    memory_clear_sreg_flag(NEGATIVE_FLAG);
    memory_set_sreg_flag(ZERO_FLAG);
    global_program_counter += 2;
    INSTRUCTION_DEBUG
}


// LDI – Load Immediate
// Loads an 8-bit constant directly to register 16 to 31.
// AVR Instruction Manual page 115
void ldi() {
    uint16_t instruction = fetch_16bit_instruction(global_program_counter);
    uint16_t rd_addr = _extract_bits(instruction, EXTRACTION_PATTERN_0000000011110000) + 16;
    uint8_t k_immediate_value = _extract_bits(instruction, EXTRACTION_PATTERN_0000111100001111);
    *(uint8_t*)(_data_mem_ptr + rd_addr) = k_immediate_value;
    global_program_counter += 2;
    INSTRUCTION_DEBUG
}


// OUT – Store Register to I/O Location
// Stores data from register Rr in the Register File to I/O Space (Ports, Timers, Configuration Registers, etc.).
// When using the I/O specific commands IN and OUT, the I/O addresses 0x00 - 0x3F must be used.
// That means 0x20 must bei added to the provided adresses.
// Atmega328 datasheet page 624
// AVR Instruction Manual page 134
void out() {
    uint16_t instruction = *(uint16_t*)(_program_mem_ptr + global_program_counter);
    uint16_t rr_addr = _extract_bits(instruction, EXTRACTION_PATTERN_0000000111110000);
    uint16_t ioa_addr = _extract_bits(instruction, EXTRACTION_PATTERN_0000011000001111) + 0x20;
    *(uint8_t*)(_data_mem_ptr + ioa_addr) = *(uint8_t*)(_data_mem_ptr + rr_addr);
    global_program_counter += 2;
    INSTRUCTION_DEBUG
}


// EOR – Exclusive OR
// Performs the logical EOR between the contents of register Rd and register Rr and places the result in the
// destination register Rd.
// AVR Instruction Manual page 91
void eor() {
    uint16_t instruction = *(uint16_t*)(_program_mem_ptr + global_program_counter);
    uint16_t rd_addr = _extract_bits(instruction, EXTRACTION_PATTERN_0000000111110000);
    uint16_t rr_addr = _extract_bits(instruction, EXTRACTION_PATTERN_0000001000001111);
    uint8_t rd_content = *(uint8_t*)(_data_mem_ptr + rd_addr);
    uint8_t rr_content = *(uint8_t*)(_data_mem_ptr + rr_addr);
    uint8_t result = rd_content + rr_content;
    *(uint8_t*)(_data_mem_ptr + rd_addr) = result;
    if (memory_get_sreg_flag(NEGATIVE_FLAG) ^ memory_get_sreg_flag(TWOS_COMPLEMENT_OVERFLOW_FLAG)) {
        memory_set_sreg_flag(SIGN_BIT);
    } else {
        memory_clear_sreg_flag(SIGN_BIT);
    }
    memory_clear_sreg_flag(TWOS_COMPLEMENT_OVERFLOW_FLAG);

    if (get_bit(result, 7)) {
        memory_set_sreg_flag(NEGATIVE_FLAG);
    } else {
        memory_clear_sreg_flag(NEGATIVE_FLAG);
    }
    if (result == 0) {
        memory_set_sreg_flag(ZERO_FLAG);
    } else {
        memory_clear_sreg_flag(ZERO_FLAG);
    }
    global_program_counter += 2;
    INSTRUCTION_DEBUG
}