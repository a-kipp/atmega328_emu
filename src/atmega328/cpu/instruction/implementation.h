#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../utility_functions.h"
#include "../../memory/memory.h"
#include "decode.h"
#include "instruction.h"
;


// unknown opcode
void unknown() {
    uint16_t instruction = mem_programMemoryFetchInstruction(mem_programCounter);

    mem_programCounter += 1;
    g_cpuCycleCount += 1;
    //exit(-1);
}



// ADC - Add with Carry
// 16-bit Opcode: 0001 11rd dddd rrrr
// Adds two registers and the contents of the C Flag and places the result in the destination register Rd.
// AVR Instruction Manual page 30
void adc() {
    uint16_t instruction = mem_programMemoryFetchInstruction(mem_programCounter);
    uint16_t rd_addr = dec_extractBits0000000111110000(instruction);
    uint16_t rr_addr = dec_extractBits0000001000001111(instruction);
    uint8_t rdContent = mem_dataMemoryRead8bit(rd_addr);
    uint8_t rrContent = mem_dataMemoryRead8bit(rr_addr);
    uint8_t result = rdContent + rrContent + (mem_sregCarryFlagC == 0 ? 0 : 1);

    bool rdBit3 = uti_getBit(rdContent, 3);
    bool rrBit3 = uti_getBit(rrContent, 3);
    bool resultBit3 = uti_getBit(result, 3);
    bool rdBit7 = uti_getBit(rdContent, 7);
    bool rrBit7 = uti_getBit(rrContent, 7);
    bool resultBit7 = uti_getBit(result, 7);

    // H: Set if there was a carry from bit 3; cleared otherwise.
    mem_sregHalfCarryFlagH = rdBit3 && rrBit3 || rrBit3 && !resultBit3 || !resultBit3 && rdBit3;

    // S = N ⊕ V, for signed tests.
    mem_sregSignBitS = mem_sregNegativeFlagN ^ mem_sregTwoComplementsOverflowFlagV;

    // V: Set if two’s complement overflow resulted from the operation; cleared otherwise.
    mem_sregTwoComplementsOverflowFlagV = rdBit7 && rrBit7 && !resultBit7 || !rdBit7 && !rrBit7 && resultBit7;

    // N: Set if MSB of the result is set; cleared otherwise.
    mem_sregNegativeFlagN = resultBit7;

    // Z: Set if the result is $00; cleared otherwise.
    mem_sregZeroFlagZ = (result == 0);

    // C: Set if there was carry from the MSB of the result; cleared otherwise.
    mem_sregCarryFlagC = rdBit7 && rrBit7 || rrBit7 && !resultBit7 || !resultBit7 && rdBit7;
    
    mem_dataMemoryWrite8bitCpu(rd_addr, result);
    mem_programCounter += 1;
    g_cpuCycleCount += 1;
}



// ADD - Add without Carry
// 16-bit Opcode: 0000 11rd dddd rrrr
// Adds two registers without the C Flag and places the result in the destination register Rd.
// AVR Instruction Manual page 32
void add() {
    uint16_t instruction = mem_programMemoryFetchInstruction(mem_programCounter);
    uint16_t rd_addr = dec_extractBits0000000111110000(instruction);
    uint16_t rr_addr = dec_extractBits0000001000001111(instruction);
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
    mem_sregHalfCarryFlagH = rdBit3 && rrBit3 || rrBit3 && !resultBit3 || !resultBit3 && rdBit3;

    // S = N ⊕ V, for signed tests.
    mem_sregSignBitS = mem_sregNegativeFlagN ^ mem_sregTwoComplementsOverflowFlagV;

    // V: Set if two’s complement overflow resulted from the operation; cleared otherwise.
    mem_sregTwoComplementsOverflowFlagV = rdBit7 && rrBit7 && !resultBit7 || !rdBit7 && !rrBit7 && resultBit7;

    // N: Set if MSB of the result is set; cleared otherwise.
    mem_sregNegativeFlagN = resultBit7;

    // Z: Set if the result is $00; cleared otherwise.
    mem_sregZeroFlagZ = (result == 0);

    // C: Set if there was carry from the MSB of the result; cleared otherwise.
    mem_sregCarryFlagC = rdBit7 && rrBit7 || rrBit7 && !resultBit7 || !resultBit7 && rdBit7;
    
    mem_dataMemoryWrite8bitCpu(rd_addr, result);
    mem_programCounter += 1;
    g_cpuCycleCount += 1;
}



// CBI – Clear Bit in I/O Register
// 16-bit Opcode: 1001 1000 AAAA Abbb
// Clears a specified bit in an I/O register. This instruction operates on the lower 32 I/O registers –
// addresses 0-31.
// AVR Instruction Manual page 65
void cbi() {
    uint16_t instruction = mem_programMemoryFetchInstruction(mem_programCounter);
    uint16_t ioa_addr = dec_extractBits0000000001111000(instruction) + 0x20;
    uint16_t bitNum = dec_extractBits0000000000000111(instruction);
    uint8_t ioaContent = mem_dataMemoryRead8bit(ioa_addr);

    mem_dataMemoryWrite8bitCpu(ioa_addr, ioaContent ^ (1 << bitNum));
    mem_programCounter += 1;
    g_cpuCycleCount += 1;
}



// EOR – Exclusive OR
// CLR – Clear Register
// 16-bit Opcode: 0010 01rd dddd rrrr
// Performs the logical EOR between the contents of register Rd and register Rr and places the result in the
// destination register Rd.
// If the both registers are the same this instruction becomes the clr instruction.
// AVR Instruction Manual page 91 and 71
void eor() {
    uint16_t instruction = mem_programMemoryFetchInstruction(mem_programCounter);
    uint16_t rd_addr = dec_extractBits0000000111110000(instruction);
    uint16_t rr_addr = dec_extractBits0000001000001111(instruction);
    uint8_t rdContent = mem_dataMemoryRead8bit(rd_addr);
    uint8_t rrContent = mem_dataMemoryRead8bit(rr_addr);
    uint8_t result = rdContent ^ rrContent;

    bool resultBit7 = uti_getBit(result, 7);

    // S: N ⊕ V, for signed tests.
    mem_sregSignBitS = mem_sregNegativeFlagN ^ mem_sregTwoComplementsOverflowFlagV;

    // V: Cleared.
    mem_sregTwoComplementsOverflowFlagV = false;

    // N: Set if MSB of the result is set; cleared otherwise.
    mem_sregNegativeFlagN = resultBit7;

    // Z is set if the result is $00; cleared otherwise.
    mem_sregZeroFlagZ = (result == 0);

    mem_dataMemoryWrite8bitCpu(rd_addr, result);
    mem_programCounter += 1;
    g_cpuCycleCount += 1;
}


// IN - Load an I/O Location to Register
// 16-bit Opcode: 1011 0AAd dddd AAAA
// Loads data from the I/O Space (Ports, Timers, Configuration Registers, etc.) into register Rd in the
// Register File.
// AVR Instruction Manual page 100
void in() {
    uint16_t instruction = mem_programMemoryFetchInstruction(mem_programCounter);
    uint16_t ioa_addr = dec_extractBits0000011000001111(instruction) + 0x20;
    uint16_t rd_addr =  dec_extractBits0000000111110000(instruction);
    uint8_t ioaContent = mem_dataMemoryRead8bit(ioa_addr);

    mem_dataMemoryWrite8bitCpu(rd_addr, ioaContent);
    mem_programCounter += 1;
    g_cpuCycleCount += 1;
}


// LDI – Load Immediate
// 16-bit Opcode: 1110 KKKK dddd KKKK
// Loads an 8-bit constant directly to register 16 to 31.
// AVR Instruction Manual page 115
void ldi() {
    uint16_t instruction = mem_programMemoryFetchInstruction(mem_programCounter);
    uint16_t rd_addr =  dec_extractBits0000000011110000(instruction) + 16;
    uint8_t constData = dec_extractBits0000111100001111(instruction);

    mem_dataMemoryWrite8bitCpu(rd_addr, constData);
    mem_programCounter += 1;
    g_cpuCycleCount += 1;
}


// NOP - No Operation
// 16-bit Opcode: 0000 0000 0000 0000
// This instruction performs a single cycle No Operation.
// AVR Instruction Manual page 131
void nop() {
    uint16_t instruction = mem_programMemoryFetchInstruction(mem_programCounter);

    mem_programCounter += 1;
    g_cpuCycleCount += 1;
}



// OUT – Store Register to I/O Location
// 16-bit Opcode: 1011 1AAr rrrr AAAA
// Stores data from register Rr in the Register File to I/O Space (Ports, Timers, Configuration Registers, etc.).
// When using the I/O specific commands IN and OUT, the I/O addresses 0x00 - 0x3F must be used.
// That means 0x20 must bei added to the provided adresses.
// Atmega328 datasheet page 624
// AVR Instruction Manual page 134
void out() {
    uint16_t instruction = mem_programMemoryFetchInstruction(mem_programCounter);
    uint16_t ioa_addr = dec_extractBits0000011000001111(instruction) + 0x20;
    uint16_t rr_addr =  dec_extractBits0000000111110000(instruction);
    uint8_t rrContent = mem_dataMemoryRead8bit(rr_addr);

    mem_dataMemoryWrite8bitCpu(ioa_addr, rrContent);
    mem_programCounter += 1;
    g_cpuCycleCount += 1;
}



// TODO
// SBIC – Skip if Bit in I/O Register is Cleared
// This instruction tests a single bit in an I/O Register and skips the next instruction if the bit is cleared. This
// instruction operates on the lower 32 I/O Registers – addresses 0-31.
// AVR Instruction Manual page 152
void sbic() {
    uint16_t instruction = mem_programMemoryFetchInstruction(mem_programCounter);
    uint16_t ioa_addr = dec_extractBits0000000001111000(instruction) + 0x20;
    uint16_t bitNum = dec_extractBits0000000000000111(instruction);
    uint8_t ioaContent = mem_dataMemoryRead8bit(ioa_addr);

    if (!ioaContent & (1<<bitNum)) {
        mem_programCounter += (1 + ins_getInstructionLength(mem_programCounter));
    } else {
        mem_programCounter += 1;
    }

    g_cpuCycleCount += 1;
}



// SBIW – Subtract Immediate from Word
// 16-bit Opcode: 1001 0111 KKdd KKKK
// Subtracts an immediate value (0-63) from a register pair and places the result in the register pair.
// This instruction operates on the upper four register pairs, and is well suited for operations on the
// Pointer Registers.
// AVR Instruction Manual page 154
void sbiw() {
    uint16_t instruction = mem_programMemoryFetchInstruction(mem_programCounter);
    uint16_t rd_addr =   dec_extractBits0000000000110000(instruction) * 2 + R24;
    uint16_t constData = dec_extractBits0000000011001111(instruction);
    uint16_t rdContent = mem_dataMemoryRead16bit(rd_addr);
    uint16_t result = rdContent - constData;

    bool resultBit15 = (bool)(result & (1 << 15));
    bool rdhBit7 = (bool)(rdContent & (1 << 15));

    // S = N ⊕ V, for signed tests.
    mem_sregSignBitS = mem_sregNegativeFlagN ^ mem_sregTwoComplementsOverflowFlagV;

    // V: Set if two’s complement overflow resulted from the operation; cleared otherwise.
    mem_sregTwoComplementsOverflowFlagV = (result & (1 << 15)) && !(rdContent & (1 << 7));

    // N: Set if MSB of the result is set; cleared otherwise.
    mem_sregNegativeFlagN = resultBit15;

    // Z: Set if the result is $0000; cleared otherwise.
    mem_sregZeroFlagZ = (result == 0);

    // C: Set if the absolute value of K is larger than the absolute value of Rd; cleared otherwise.
    mem_sregSignBitS = resultBit15 && rdhBit7;

    mem_dataMemoryWrite16bit(rd_addr, result);
    mem_programCounter += 1;
    g_cpuCycleCount += 2;
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
    uint16_t instruction = mem_programMemoryFetchInstruction(mem_programCounter);
    int16_t constData = (int16_t)dec_extractBits0000001111111000(instruction);

    if (mem_sregZeroFlagZ) {
        mem_programCounter += constData;
        g_cpuCycleCount += 2;
    } else {
        mem_programCounter += 1;
        g_cpuCycleCount += 1;
    }
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
    uint16_t instruction = mem_programMemoryFetchInstruction(mem_programCounter);
    uint16_t rd_addr = dec_extractBits0000000111110000(instruction);
    uint8_t rdContent = mem_dataMemoryRead8bit(rd_addr);
    uint8_t result = rdContent - 1;

    bool resultBit7 = uti_getBit(result, 7);

    // S: N ⊕ V, for signed tests.
    mem_sregSignBitS = mem_sregNegativeFlagN ^ mem_sregTwoComplementsOverflowFlagV;

    // V: Set if two’s complement overflow resulted from the operation; cleared otherwise.
    // Two’s complement overflow occurs if and only if Rd was $80 before the operation.
    mem_sregTwoComplementsOverflowFlagV = (rdContent == 0x80);

    // N: Set if MSB of the result is set; cleared otherwise.
    mem_sregNegativeFlagN = resultBit7;

    // Z is set if the result is $00; cleared otherwise.
    mem_sregZeroFlagZ = (result == 0);

    mem_dataMemoryWrite8bitCpu(rd_addr, result);
    mem_programCounter += 1;
    g_cpuCycleCount += 1;
}



// RJMP – Relative Jump
// 16-bit Opcode: 1100 kkkk kkkk kkkk
// Relative jump to an address within PC - 2K +1 and PC + 2K (words). For AVR microcontrollers with
// Program memory not exceeding 4K words (8KB) this instruction can address the entire memory from
// every address location. See also JMP.
// AVR Instruction Manual page 142
void rjmp() {
    uint16_t instruction = mem_programMemoryFetchInstruction(mem_programCounter);
    int16_t constAddress = (int16_t)dec_extractBits0000111111111111(instruction);
    uint16_t jumpDest_addr = (mem_programCounter + constAddress - 0xfff) % (DATA_MEMORY_END + 1);

    mem_programCounter = jumpDest_addr;
    
    g_cpuCycleCount += 2;
}



// STS – Store Direct to Data Space
// 16-bit Opcode: 1001 001d dddd 0000
// Stores one byte from a Register to the data space. For parts with SRAM, the data space consists of the
// Register File, I/O memory, and internal SRAM (and external SRAM if applicable). For parts without
// SRAM, the data space consists of the Register File only. The EEPROM has a separate address space.
// A 16-bit address must be supplied. Memory access is limited to the current data segment of 64KB. The
// STS instruction uses the RAMPD Register to access memory above 64KB. To access another data
// segment in devices with more than 64KB data space, the RAMPD in register in the I/O area has to be
// changed.
// This instruction is not available in all devices. Refer to the device specific instruction set summary.
void sts() {
    uint16_t instructionFirst = mem_programMemoryFetchInstruction(mem_programCounter);
    uint16_t rr_addr = dec_extractBits0000000111110000(instructionFirst);
    uint16_t instructionSecond = mem_programMemoryFetchInstruction(mem_programCounter + 1);
    uint16_t mem_addr = instructionSecond;
    uint8_t memContent = mem_dataMemoryRead8bit(rr_addr);

    mem_dataMemoryWrite8bitCpu(mem_addr, memContent);
    mem_programCounter += 1;
    g_cpuCycleCount += 1;
}



// CT - Compare
// 16-bit Opcode: 0001 01rd dddd rrrr
// This instruction performs a compare between two registers Rd and Rr. None of the registers are changed.
// All conditional branches can be used after this instruction.
void cp() {
    uint16_t instructionFirst = mem_programMemoryFetchInstruction(mem_programCounter);
    uint16_t rd_addr = dec_extractBits0000000111110000(instructionFirst);
    uint16_t rr_addr = dec_extractBits0000001000001111(instructionFirst);
    uint8_t rdContent = mem_dataMemoryRead8bit(rd_addr);
    uint8_t rrContent = mem_dataMemoryRead8bit(rr_addr);
    uint8_t result = rdContent - rrContent;

    bool rdBit3 = uti_getBit(rdContent, 3);
    bool rrBit3 = uti_getBit(rrContent, 3);
    bool resultBit3 = uti_getBit(result, 3);
    bool rdBit7 = uti_getBit(rdContent, 7);
    bool rrBit7 = uti_getBit(rrContent, 7);
    bool resultBit7 = uti_getBit(result, 7);

    // H: Set if there was a borrow from bit 3; cleared otherwise.
    mem_sregHalfCarryFlagH = !rdBit3 && rrBit3 || rrBit3 && resultBit3 || resultBit3 && rdBit3;

    // S: N ⊕ V, for signed tests.
    mem_sregSignBitS = mem_sregNegativeFlagN ^ mem_sregTwoComplementsOverflowFlagV;

    // V: Set if two’s complement overflow resulted from the operation; cleared otherwise.
    mem_sregTwoComplementsOverflowFlagV = rdBit7 && !rrBit7 && resultBit7 || !rdBit7 && !rrBit7 && resultBit7;

    // N: Set if MSB of the result is set; cleared otherwise.
    mem_sregNegativeFlagN = resultBit7;

    // Z: Set if the result is $00; cleared otherwise.
    mem_sregZeroFlagZ = (result == 0);

    // C: Set if the absolute value of the contents of Rr is larger than the absolute value of Rd; cleared
    // otherwise.
    mem_sregCarryFlagC = rrContent > rdContent;

    mem_programCounter += 1;
    g_cpuCycleCount += 1;
}


