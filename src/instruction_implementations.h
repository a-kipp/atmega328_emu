#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "global_variables.h"
#include "memory.h"
#include "utility_functions.h"

#define INSTRUCTION_DEBUG printf(instructionName);


// Extraction Patterns
#define EXTRACTION_PATTERN_0000000111110000 0
#define EXTRACTION_PATTERN_0000000011110000 1
#define EXTRACTION_PATTERN_0000111100001111 2
#define EXTRACTION_PATTERN_0000011000001111 3
#define EXTRACTION_PATTERN_0000001000001111 4
#define EXTRACTION_PATTERN_0000000000110000 5
#define EXTRACTION_PATTERN_0000000011001111 6
#define EXTRACTION_PATTERN_0000001111111000 7
#define EXTRACTION_PATTERN_0000111111111111 8


// extract addresses and immediate values from an instruction
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

    case EXTRACTION_PATTERN_0000001111111000:
        return (instruction & 0b0000001111111000) >> 3; break;

    case EXTRACTION_PATTERN_0000111111111111:
        return (instruction & 0b0000111111111111); break;

    default:
        break;
    }
}



// Instruction Implementations
// ____________________________________________________________________________________________________________________


// unknown opcode
void unknown() {
    char *instructionName = "unknown";
    printf("unknown instruction");
    exit(-1);
    INSTRUCTION_DEBUG   
}


// NOP - No Operation
// 16-bit Opcode: 0000 0000 0000 0000
// This instruction performs a single cycle No Operation.
// AVR Instruction Manual page 131
void nop() {
    char *instructionName = "nop";
    g_programCounter += 1;
    g_cycles += 1;
    INSTRUCTION_DEBUG
}


// ADC - Add with Carry
// 16-bit Opcode: 0001 11rd dddd rrrr
// Adds two registers and the contents of the C Flag and places the result in the destination register Rd.
// AVR Instruction Manual page 30
void adc() {
    char *instructionName = "adc";
    uint16_t instruction = mem_programMemoryFetchInstruction(g_programCounter);
    uint16_t rd_addr = _extract_bits(instruction, EXTRACTION_PATTERN_0000000111110000);
    uint16_t rr_addr = _extract_bits(instruction, EXTRACTION_PATTERN_0000001000001111);
    uint8_t rdContent = mem_dataMemoryRead8bit(rd_addr);
    uint8_t rrContent = mem_dataMemoryRead8bit(rr_addr);
    uint8_t result = rdContent + rrContent + (uint8_t)mem_getSregCarryFlag();

    bool rdBit3 = uti_getBit(rdContent, 3);
    bool rrBit3 = uti_getBit(rrContent, 3);
    bool resultBit3 = uti_getBit(result, 3);
    bool rdBit7 = uti_getBit(rdContent, 7);
    bool rrBit7 = uti_getBit(rrContent, 7);
    bool resultBit7 = uti_getBit(result, 7);

    // H: Set if there was a carry from bit 3; cleared otherwise.
    mem_setSregCarryFlagTo(rdBit3 && rrBit3 || rrBit3 && !resultBit3 || !resultBit3 && rdBit3);

    // S = N ⊕ V, for signed tests.
    mem_setSregSignBitTo(mem_getSregZeroFlag() ^ mem_getSregNegativeFlag());

    // V: Set if two’s complement overflow resulted from the operation; cleared otherwise.
    mem_setSregTwosComplementOverflowFlagTo(rdBit7 && rrBit7 && !resultBit7 || !rdBit7 && !rrBit7 && resultBit7);

    // N: Set if MSB of the result is set; cleared otherwise.
    mem_setSregNegativeFlagTo(resultBit7);

    // Z: Set if the result is $00; cleared otherwise.
    mem_setSregZeroFlagTo(result == 0x00);

    // C: Set if there was carry from the MSB of the result; cleared otherwise.
    mem_setSregCarryFlagTo(rdBit7 && rrBit7 || rrBit7 && !resultBit7 || !resultBit7 && rdBit7);
    
    mem_dataMemoryWrite8bit(rd_addr, result);
    g_programCounter += 1;
    g_cycles += 1;
    INSTRUCTION_DEBUG
}


// ADD - Add without Carry
// 16-bit Opcode: 0000 11rd dddd rrrr
// Adds two registers without the C Flag and places the result in the destination register Rd.
// AVR Instruction Manual page 32
void add() {
    char *instructionName = "add";
    uint16_t instruction = mem_programMemoryFetchInstruction(g_programCounter);
    uint16_t rd_addr = _extract_bits(instruction, EXTRACTION_PATTERN_0000000111110000);
    uint16_t rr_addr = _extract_bits(instruction, EXTRACTION_PATTERN_0000001000001111);
    uint8_t rdContent = mem_dataMemoryRead8bit(rd_addr);
    uint8_t rrContent = mem_dataMemoryRead8bit(rr_addr);
    uint8_t result = rdContent + rrContent;

    bool rdBit3 = uti_getBit(rdContent, 3);
    bool rrBit3 = uti_getBit(rrContent, 3);
    bool resultBit3 = uti_getBit(result, 3);
    bool rdBit7 = uti_getBit(rdContent, 7);
    bool rrBit7 = uti_getBit(rrContent, 7);
    bool resultBit7 = uti_getBit(result, 7);

    // H: Set if there was a carry from bit 3; cleared otherwise.
    mem_setSregCarryFlagTo(rdBit3 && rrBit3 || rrBit3 && !resultBit3 || !resultBit3 && rdBit3);

    // S = N ⊕ V, for signed tests.
    mem_setSregSignBitTo(mem_getSregZeroFlag() ^ mem_getSregNegativeFlag());

    // V: Set if two’s complement overflow resulted from the operation; cleared otherwise.
    mem_setSregTwosComplementOverflowFlagTo(rdBit7 && rrBit7 && !resultBit7 || !rdBit7 && !rrBit7 && resultBit7);

    // N: Set if MSB of the result is set; cleared otherwise.
    mem_setSregNegativeFlagTo(resultBit7);

    // Z: Set if the result is $00; cleared otherwise.
    mem_setSregZeroFlagTo(result == 0x00);

    // C: Set if there was carry from the MSB of the result; cleared otherwise.
    mem_setSregCarryFlagTo(rdBit7 && rrBit7 || rrBit7 && !resultBit7 || !resultBit7 && rdBit7);
    
    mem_dataMemoryWrite8bit(rd_addr, result);
    g_programCounter += 1;
    g_cycles += 1;
    INSTRUCTION_DEBUG
}


// CLR – Clear Register
// 16-bit Opcode: 0010 01dd dddd dddd
// Clears a register. This instruction performs an Exclusive OR between a register and itself. This will clear all bits in the register.
// AVR Instruction Manual page 71
void clr() {
    char *instructionName = "clr";
    uint16_t instruction = mem_programMemoryFetchInstruction(g_programCounter);
    uint16_t rd_addr = _extract_bits(instruction, EXTRACTION_PATTERN_0000000111110000);

    // S: Cleared.
    mem_setSregSignBitTo(false);

    // V: Cleared.
    mem_setSregTwosComplementOverflowFlagTo(false);

    // N: Cleared.
    mem_setSregNegativeFlagTo(false);

    // Z:Set.
    mem_setSregZeroFlagTo(true);

    mem_dataMemoryWrite8bit(rd_addr, 0);
    g_programCounter += 1;
    g_cycles += 1;
    INSTRUCTION_DEBUG
}


// LDI – Load Immediate
// 16-bit Opcode: 1110 KKKK dddd KKKK
// Loads an 8-bit constant directly to register 16 to 31.
// AVR Instruction Manual page 115
void ldi() {
    char *instructionName = "ldi";
    uint16_t instruction = mem_programMemoryFetchInstruction(g_programCounter);
    uint16_t rd_addr =  _extract_bits(instruction, EXTRACTION_PATTERN_0000000011110000) + 16;
    uint8_t constData = _extract_bits(instruction, EXTRACTION_PATTERN_0000111100001111);

    mem_dataMemoryWrite8bit(rd_addr, constData);
    g_programCounter += 1;
    g_cycles += 1;
    INSTRUCTION_DEBUG
}


// OUT – Store Register to I/O Location
// 16-bit Opcode: 1011 1AAr rrrr AAAA
// Stores data from register Rr in the Register File to I/O Space (Ports, Timers, Configuration Registers, etc.).
// When using the I/O specific commands IN and OUT, the I/O addresses 0x00 - 0x3F must be used.
// That means 0x20 must bei added to the provided adresses.
// Atmega328 datasheet page 624
// AVR Instruction Manual page 134
void out() {
    char *instructionName = "out";
    uint16_t instruction = mem_programMemoryFetchInstruction(g_programCounter);
    uint16_t rr_addr =  _extract_bits(instruction, EXTRACTION_PATTERN_0000000111110000);
    uint16_t ioa_addr = _extract_bits(instruction, EXTRACTION_PATTERN_0000011000001111) + 0x20;
    uint8_t rrContent = mem_dataMemoryRead8bit(rr_addr);
    
    mem_dataMemoryWrite8bit(ioa_addr, rrContent);
    g_programCounter += 1;
    g_cycles += 1;
    INSTRUCTION_DEBUG
}


// EOR – Exclusive OR
// 16-bit Opcode: 0010 01rd dddd rrrr
// Performs the logical EOR between the contents of register Rd and register Rr and places the result in the
// destination register Rd.
// AVR Instruction Manual page 91
void eor() {
    char *instructionName = "eor";
    uint16_t instruction = mem_programMemoryFetchInstruction(g_programCounter);
    uint16_t rd_addr = _extract_bits(instruction, EXTRACTION_PATTERN_0000000111110000);
    uint16_t rr_addr = _extract_bits(instruction, EXTRACTION_PATTERN_0000001000001111);
    uint8_t rdContent = mem_dataMemoryRead8bit(rd_addr);
    uint8_t rrContent = mem_dataMemoryRead8bit(rr_addr);
    uint8_t result = rdContent ^ rrContent;

    bool resultBit7 = uti_getBit(result, 7);

    // S: N ⊕ V, for signed tests.
    mem_setSregSignBitTo(mem_getSregZeroFlag() ^ mem_getSregNegativeFlag());

    // V: Cleared.
    mem_setSregTwosComplementOverflowFlagTo(false);

    // N: Set if MSB of the result is set; cleared otherwise.
    mem_setSregNegativeFlagTo(resultBit7);

    // Z is set if the result is $00; cleared otherwise.
    mem_setSregZeroFlagTo(result == 0x00);

    mem_dataMemoryWrite8bit(rd_addr, result);
    g_programCounter += 1;
    g_cycles += 1;
    INSTRUCTION_DEBUG
}


// SBIW – Subtract Immediate from Word
// 16-bit Opcode: 1001 0111 KKdd KKKK
// Subtracts an immediate value (0-63) from a register pair and places the result in the register pair.
// This instruction operates on the upper four register pairs, and is well suited for operations on the
// Pointer Registers.
// AVR Instruction Manual page 154
void sbiw() {
    char *instructionName = "sbiw";
    uint16_t instruction = mem_programMemoryFetchInstruction(g_programCounter);
    uint16_t rd_addr =   _extract_bits(instruction, EXTRACTION_PATTERN_0000000000110000) * 2 + R24;
    uint16_t constData = _extract_bits(instruction, EXTRACTION_PATTERN_0000000011001111);
    uint16_t rdContent = mem_dataMemoryRead16bit(rd_addr);
    uint16_t result = rdContent - constData;

    bool resultBit15 = (bool)(result & (1 << 15));
    bool rdhBit7 = (bool)(rdContent & (1 << 15));

    // S = N ⊕ V, for signed tests.
    mem_setSregSignBitTo(mem_getSregZeroFlag() ^ mem_getSregNegativeFlag());

    // V: Set if two’s complement overflow resulted from the operation; cleared otherwise.
    mem_setSregTwosComplementOverflowFlagTo((result & (1 << 15)) && !(rdContent & (1 << 7)));

    // N: Set if MSB of the result is set; cleared otherwise.
    mem_setSregNegativeFlagTo(resultBit15);

    // Z: Set if the result is $0000; cleared otherwise.
    mem_setSregZeroFlagTo(result == 0x0000);

    // C: Set if the absolute value of K is larger than the absolute value of Rd; cleared otherwise.
    mem_setSregSignBitTo(resultBit15 && rdhBit7);

    mem_dataMemoryWrite16bit(rd_addr, result);
    g_programCounter += 1;
    g_cycles += 2;
    INSTRUCTION_DEBUG   
}


// BRNE – Branch if Not Equal
// 16-bit Opcode: 1111 01kk kkkk k001
// Conditional relative branch. Tests the Zero Flag (Z) and branches relatively to PC if Z is cleared. If the
// instruction is executed immediately after any of the instructions CP, CPI, SUB, or SUBI, the branch will
// occur if and only if, the unsigned or signed binary number represented in Rd was not equal to the
// unsigned or signed binary number represented in Rr. This instruction branches relatively to PC in either
// direction (PC - 63 ≤ destination ≤ PC + 64). Parameter k is the offset from PC and is represented in two’s
// complement form. (Equivalent to instruction BRBC 1,k.)
// AVR Instruction Manual page 54
void brne() {
    char *instructionName = "brne";
    uint16_t instruction = mem_programMemoryFetchInstruction(g_programCounter);
    uint16_t constData = _extract_bits(instruction, EXTRACTION_PATTERN_0000001111111000);

    if (mem_getSregZeroFlag()) {
        g_programCounter += (constData + 1);
        g_cycles += 2;
    } else {
        g_programCounter += 1;
        g_cycles += 1;
    }
    INSTRUCTION_DEBUG 
}


// DEC – Decrement
// 16-bit Opcode: 1001 010d dddd 1010
// Subtracts one -1- from the contents of register Rd and places the result in the destination register Rd.
// The C Flag in SREG is not affected by the operation, thus allowing the DEC instruction to be used on a
// loop counter in multiple-precision computations.
// When operating on unsigned values, only BREQ and BRNE branches can be expected to perform
// consistently. When operating on two’s complement values, all signed branches are available.
// AVR Instruction Manual page 84
void dec() {
    char *instructionName = "dec";
    uint16_t instruction = mem_programMemoryFetchInstruction(g_programCounter);
    uint16_t rd_addr = _extract_bits(instruction, EXTRACTION_PATTERN_0000000111110000);
    uint8_t rdContent = mem_dataMemoryRead8bit(rd_addr);
    uint8_t result = rdContent - 1;

    bool resultBit7 = uti_getBit(result, 7);

    // S: N ⊕ V, for signed tests.
    mem_setSregSignBitTo(mem_getSregZeroFlag() ^ mem_getSregNegativeFlag());

    // V: Set if two’s complement overflow resulted from the operation; cleared otherwise.
    // Two’s complement overflow occurs if and only if Rd was $80 before the operation.
    mem_setSregTwosComplementOverflowFlagTo(rdContent == 0x80);

    // N: Set if MSB of the result is set; cleared otherwise.
    mem_setSregNegativeFlagTo(resultBit7);

    // Z is set if the result is $00; cleared otherwise.
    mem_setSregZeroFlagTo(result == 0x00);

    mem_dataMemoryWrite8bit(rd_addr, result);
    g_programCounter += 1;
    g_cycles += 1;
    INSTRUCTION_DEBUG
}


// RJMP – Relative Jump
// 16-bit Opcode: 1100 kkkk kkkk kkkk
// Relative jump to an address within PC - 2K +1 and PC + 2K (words). For AVR microcontrollers with
// Program memory not exceeding 4K words (8KB) this instruction can address the entire memory from
// every address location. See also JMP.
// AVR Instruction Manual page 142
void rjmp() {
    char *instructionName = "rjmp";
    uint16_t instruction = mem_programMemoryFetchInstruction(g_programCounter);
    int16_t constAddress = (int16_t)_extract_bits(instruction, EXTRACTION_PATTERN_0000111111111111);

    g_programCounter += (constAddress + 1) % (DATA_MEMORY_END + 1);
    g_cycles += 2;
    INSTRUCTION_DEBUG   
}
