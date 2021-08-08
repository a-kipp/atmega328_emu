#include <stdint.h>
#include "global_variables.h"
#include "memory.h"
#include "utility_functions.h"

#define INSTRUCTION_DEBUG 


// Extraction Patterns
#define EXTRACTION_PATTERN_0000000111110000 0
#define EXTRACTION_PATTERN_0000000011110000 1
#define EXTRACTION_PATTERN_0000111100001111 2
#define EXTRACTION_PATTERN_0000011000001111 3
#define EXTRACTION_PATTERN_0000001000001111 4
#define EXTRACTION_PATTERN_0000000000110000 5
#define EXTRACTION_PATTERN_0000000011001111 6


// extracts addresses and immediate values from an instruction
uint16_t _extract_bits(uint16_t instruction, int extractionPattern) {
    switch (extractionPattern) {
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

    case EXTRACTION_PATTERN_0000000000110000:
        return (instruction & 0b0000000000110000) >> 4; break;

    case EXTRACTION_PATTERN_0000000011001111:
        return (instruction & 0b0000000011000000) >> 6 | (instruction & 0b0000000000001111); break;   

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
    g_programCounter += 1;
}


// ADC - Add with Carry
// Adds two registers and the contents of the C Flag and places the result in the destination register Rd.
void adc(uint8_t rd_addr, uint8_t rr_addr) {
    uint8_t rd = *(_dataMemory_ptr + rd_addr);
    uint8_t rr = *(_dataMemory_ptr + rr_addr);
    uint8_t result = rd + rr + mem_getSregFlag(CARRY_FLAG);

    if (uti_extractSingleBit(rd, 3) && uti_extractSingleBit(rr, 3) || uti_extractSingleBit(rr, 3) && !uti_extractSingleBit(result, 3) || !uti_extractSingleBit(result, 3) && uti_extractSingleBit(rd, 3)) {
        mem_setSregFlag(CARRY_FLAG);
    } else {
        mem_clearSregFlag(CARRY_FLAG);
    }
    if (mem_getSregFlag(NEGATIVE_FLAG) ^ mem_getSregFlag(TWOS_COMPLEMENT_OVERFLOW_FLAG)) {
        mem_setSregFlag(SIGN_BIT);
    } else {
        mem_clearSregFlag(SIGN_BIT);
    }
    if (uti_extractSingleBit(rd, 7) && uti_extractSingleBit(rr, 7) && !uti_extractSingleBit(result, 7) || !uti_extractSingleBit(rd, 7) && !uti_extractSingleBit(rr, 7) && uti_extractSingleBit(result, 7)) {
        mem_setSregFlag(TWOS_COMPLEMENT_OVERFLOW_FLAG);
    } else {
        mem_clearSregFlag(TWOS_COMPLEMENT_OVERFLOW_FLAG)
    }
    if (uti_extractSingleBit(result, 7)) {
        mem_setSregFlag(NEGATIVE_FLAG);
    } else {
        mem_clearSregFlag(NEGATIVE_FLAG);
    }
    if (result == 0) {
        mem_setSregFlag(ZERO_FLAG);
    } else {
        mem_clearSregFlag(ZERO_FLAG);
    }
    if (uti_extractSingleBit(rd, 7) && uti_extractSingleBit(rr, 7) || uti_extractSingleBit(rr, 7) && uti_extractSingleBit(result, 7) || !uti_extractSingleBit(result, 7) && !uti_extractSingleBit(result, 7)) {
        mem_setSregFlag(CARRY_FLAG);
    } else {
        mem_clearSregFlag(CARRY_FLAG);
    }
    
    *(_dataMemory_ptr + rd_addr) = result;
    g_programCounter += 1;
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
    uint16_t instruction = mem_programMemoryFetchInstruction(g_programCounter);
    uint16_t rd_addr = _extract_bits(instruction, EXTRACTION_PATTERN_0000000111110000);
    *(uint8_t*)(_dataMemory_ptr + rd_addr) = 0;
    mem_clearSregFlag(SIGN_BIT);
    mem_clearSregFlag(TWOS_COMPLEMENT_OVERFLOW_FLAG);
    mem_clearSregFlag(NEGATIVE_FLAG);
    mem_setSregFlag(ZERO_FLAG);
    g_programCounter += 1;
    INSTRUCTION_DEBUG
}


// LDI – Load Immediate
// Loads an 8-bit constant directly to register 16 to 31.
// AVR Instruction Manual page 115
void ldi() {
    uint16_t instruction = mem_programMemoryFetchInstruction(g_programCounter);
    uint16_t rd_addr = _extract_bits(instruction, EXTRACTION_PATTERN_0000000011110000) + 16;
    uint8_t k_immediate_value = _extract_bits(instruction, EXTRACTION_PATTERN_0000111100001111);
    *(uint8_t*)(_dataMemory_ptr + rd_addr) = k_immediate_value;
    g_programCounter += 1;
    INSTRUCTION_DEBUG
}


// OUT – Store Register to I/O Location
// Stores data from register Rr in the Register File to I/O Space (Ports, Timers, Configuration Registers, etc.).
// When using the I/O specific commands IN and OUT, the I/O addresses 0x00 - 0x3F must be used.
// That means 0x20 must bei added to the provided adresses.
// Atmega328 datasheet page 624
// AVR Instruction Manual page 134
void out() {
    uint16_t instruction = mem_programMemoryFetchInstruction(g_programCounter);
    uint16_t rr_addr = _extract_bits(instruction, EXTRACTION_PATTERN_0000000111110000);
    uint16_t ioa_addr = _extract_bits(instruction, EXTRACTION_PATTERN_0000011000001111) + 0x20;
    *(uint8_t*)(_dataMemory_ptr + ioa_addr) = *(uint8_t*)(_dataMemory_ptr + rr_addr);
    g_programCounter += 1;
    INSTRUCTION_DEBUG
}


// EOR – Exclusive OR
// Performs the logical EOR between the contents of register Rd and register Rr and places the result in the
// destination register Rd.
// AVR Instruction Manual page 91
void eor() {
    uint16_t instruction = mem_programMemoryFetchInstruction(g_programCounter);
    uint16_t rd_addr = _extract_bits(instruction, EXTRACTION_PATTERN_0000000111110000);
    uint16_t rr_addr = _extract_bits(instruction, EXTRACTION_PATTERN_0000001000001111);
    uint8_t rd_content = *(uint8_t*)(_dataMemory_ptr + rd_addr);
    uint8_t rr_content = *(uint8_t*)(_dataMemory_ptr + rr_addr);
    uint8_t result = rd_content + rr_content;
    *(uint8_t*)(_dataMemory_ptr + rd_addr) = result;
    if (mem_getSregFlag(NEGATIVE_FLAG) ^ mem_getSregFlag(TWOS_COMPLEMENT_OVERFLOW_FLAG)) {
        mem_setSregFlag(SIGN_BIT);
    } else {
        mem_clearSregFlag(SIGN_BIT);
    }
    mem_clearSregFlag(TWOS_COMPLEMENT_OVERFLOW_FLAG);

    if (uti_extractSingleBit(result, 7)) {
        mem_setSregFlag(NEGATIVE_FLAG);
    } else {
        mem_clearSregFlag(NEGATIVE_FLAG);
    }
    if (result == 0) {
        mem_setSregFlag(ZERO_FLAG);
    } else {
        mem_clearSregFlag(ZERO_FLAG);
    }
    g_programCounter += 1;
    INSTRUCTION_DEBUG
}


// SBIW – Subtract Immediate from Word
// Subtracts an immediate value (0-63) from a register pair and places the result in the register pair.
// This instruction operates on the upper four register pairs, and is well suited for operations on the
// Pointer Registers.
// AVR Instruction Manual page 91
void sbiw() {
    uint16_t instruction = mem_programMemoryFetchInstruction(g_programCounter);
    uint16_t extractedValue = _extract_bits(instruction, EXTRACTION_PATTERN_0000000000110000);
    uint16_t destRegister_addr = extractedValue * 2 + R24;
    uint16_t constData = _extract_bits(instruction, EXTRACTION_PATTERN_0000000011001111);
    uint16_t result = mem_dataMemoryRead16bit(destRegister_addr) - constData;
    // TODO
}
