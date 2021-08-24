#include <stdint.h>
#include <stdbool.h>
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
    exit(-1);
    INSTRUCTION_DEBUG   
}


// NOP - No Operation
// This instruction performs a single cycle No Operation.
// AVR Instruction Manual page 131
// 16-bit Opcode: 0000 0000 0000 0000
void nop() {
    g_programCounter += 1;
    INSTRUCTION_DEBUG
}


// ADC - Add with Carry
// Adds two registers and the contents of the C Flag and places the result in the destination register Rd.
// AVR Instruction Manual page 30
// 16-bit Opcode: 0001 11rd dddd rrrr
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
// AVR Instruction Manual page 30
// 16-bit Opcode: 0000 11rd dddd rrrr
void add() {

}


// CLR – Clear Register
// Clears a register. This instruction performs an Exclusive OR between a register and itself. This will clear all bits in the register.
// AVR Instruction Manual page 71
// 16-bit Opcode: 0010 01dd dddd dddd
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
// 16-bit Opcode: 1110 KKKK dddd KKKK
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
// 16-bit Opcode: 1011 1AAr rrrr AAAA
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
// 16-bit Opcode: 0010 01rd dddd rrrr
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
// AVR Instruction Manual page 154
// 16-bit Opcode: 1001 0111 KKdd KKKK
void sbiw() {
    uint16_t instruction = mem_programMemoryFetchInstruction(g_programCounter);
    uint16_t extractedValue = _extract_bits(instruction, EXTRACTION_PATTERN_0000000000110000);
    uint16_t destRegister_addr = extractedValue * 2 + R24;
    uint16_t registerContent = mem_dataMemoryRead16bit(destRegister_addr);
    uint16_t constData = _extract_bits(instruction, EXTRACTION_PATTERN_0000000011001111);
    uint16_t result = registerContent - constData;

    // S = N ⊕ V, for signed tests.
    mem_setSregSignBitTo((mem_getSregZeroFlag() ^ mem_getSregNegativeFlag());

    // V = R15 • Rdh7, Set if two’s complement overflow resulted from the operation; cleared otherwise.
    mem_setSregTwosComplementOverflowFlagTo((result & (1 << 15)) && !(registerContent & (1 << 7)));

    // N = R15;
    mem_setSregNegativeFlagTo(result & (1 << 15));

    // Set Z if the result is $0000; cleared otherwise.
    mem_setSregZeroFlagTo(result == 0x0000);

    // C R15 • Rdh7, Set if the absolute value of K is larger than the absolute value of Rd; cleared otherwise.
    mem_setSregSignBitTo(constData > registerContent);

    mem_dataMemoryWrite16bit(destRegister_addr, result);
    g_programCounter += 1;
    g_cycles += 2;
    INSTRUCTION_DEBUG   
}


// BRNE – Branch if Not Equal
// Conditional relative branch. Tests the Zero Flag (Z) and branches relatively to PC if Z is cleared. If the
// instruction is executed immediately after any of the instructions CP, CPI, SUB, or SUBI, the branch will
// occur if and only if, the unsigned or signed binary number represented in Rd was not equal to the
// unsigned or signed binary number represented in Rr. This instruction branches relatively to PC in either
// direction (PC - 63 ≤ destination ≤ PC + 64). Parameter k is the offset from PC and is represented in two’s
// complement form. (Equivalent to instruction BRBC 1,k.)
// AVR Instruction Manual page 54
// 16-bit Opcode: 1111 01kk kkkk k001
void brne() {
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
// Subtracts one -1- from the contents of register Rd and places the result in the destination register Rd.
// The C Flag in SREG is not affected by the operation, thus allowing the DEC instruction to be used on a
// loop counter in multiple-precision computations.
// When operating on unsigned values, only BREQ and BRNE branches can be expected to perform
// consistently. When operating on two’s complement values, all signed branches are available.
// AVR Instruction Manual page 84
// 16-bit Opcode: 1001 010d dddd 1010
void dec() {
    uint16_t instruction = mem_programMemoryFetchInstruction(g_programCounter);
    uint16_t destRegister_addr = _extract_bits(instruction, EXTRACTION_PATTERN_0000000111110000);
    uint8_t registerContent = mem_dataMemoryRead8bit(destRegister_addr);
    uint8_t result = registerContent - 1;

    // S = N ⊕ V, for signed tests.
    mem_setSregSignBitTo((mem_getSregZeroFlag() ^ mem_getSregNegativeFlag());

    // V is set Set if two’s complement overflow resulted from the operation; cleared otherwise.
    // Two’s complement overflow occurs if and only if Rd was $80 before the operation.
    mem_setSregTwosComplementOverflowFlagTo(registerContent == 0x80);

    // N = R7, Set if MSB of the result is set; cleared otherwise.
    mem_setSregNegativeFlagTo(result & (1 << 7));

    // Z is set if the result is $00; cleared otherwise.
    mem_setSregZeroFlagTo(result == 0x00);

    mem_dataMemoryWrite8bit(result);
    g_programCounter += 1;
    g_cycles += 1;
    INSTRUCTION_DEBUG
}


// RJMP – Relative Jump
// Relative jump to an address within PC - 2K +1 and PC + 2K (words). For AVR microcontrollers with
// Program memory not exceeding 4K words (8KB) this instruction can address the entire memory from
// every address location. See also JMP.
// AVR Instruction Manual page 142
// 16-bit Opcode: 1100 kkkk kkkk kkkk
rjmp() {
    uint16_t instruction = mem_programMemoryFetchInstruction(g_programCounter);
    int16_t constAddress = _extract_bits(instruction, EXTRACTION_PATTERN_0000111111111111);
    g_programCounter += (constAddress + 1) % (DATA_MEMORY_END + 1);
    g_cycles += 2;
    INSTRUCTION_DEBUG   
}
