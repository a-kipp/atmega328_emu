#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils.h"
#include "../memory/memory.h"
#include "decoder.h"
#include "instructions.h"
;


// unknown opcode
void unknown() {
    uint16_t instruction = mem_fetchInstruction(cpu_programCounter);

    cpu_programCounter += 1;
    cpu_incrementCycleCounter(1);
    //exit(-1);
}



// ADC - Add with Carry
// 16-bit Opcode: 0001 11rd dddd rrrr
// Adds two registers and the contents of the C Flag and places the result in the destination register Rd.
// AVR Instruction Manual page 30
void adc() {
    uint16_t instruction = mem_fetchInstruction(cpu_programCounter);
    uint16_t rd_addr = dec_extractBits0000000111110000(instruction);
    uint16_t rr_addr = dec_extractBits0000001000001111(instruction);
    uint8_t rdContent = acc_dataRead8bit(rd_addr);
    uint8_t rrContent = acc_dataRead8bit(rr_addr);
    uint8_t result = rdContent + rrContent + (reg_sregCarry == 0 ? 0 : 1);

    bool rdBit3 = uti_getBit(rdContent, 3);
    bool rrBit3 = uti_getBit(rrContent, 3);
    bool resultBit3 = uti_getBit(result, 3);
    bool rdBit7 = uti_getBit(rdContent, 7);
    bool rrBit7 = uti_getBit(rrContent, 7);
    bool resultBit7 = uti_getBit(result, 7);

    // H: Set if there was a carry from bit 3; cleared otherwise.
    reg_sregHalfCarry = (rdBit3 && rrBit3 || rrBit3 && !resultBit3 || !resultBit3 && rdBit3);

    // S = N ⊕ V, for signed tests.
    reg_sregSignBit = (reg_sregNegative ^ reg_sregTwoComplOverflow);

    // V: Set if two’s complement overflow resulted from the operation; cleared otherwise.
    reg_sregTwoComplOverflow = (rdBit7 && rrBit7 && !resultBit7 || !rdBit7 && !rrBit7 && resultBit7);

    // N: Set if MSB of the result is set; cleared otherwise.
    reg_sregNegative = (resultBit7);

    // Z: Set if the result is $00; cleared otherwise.
    reg_sregZero = (result == 0);

    // C: Set if there was carry from the MSB of the result; cleared otherwise.
    reg_sregCarry = (rdBit7 && rrBit7 || rrBit7 && !resultBit7 || !resultBit7 && rdBit7);
    
    cpu_programCounter += 1;
    cpu_incrementCycleCounter(1);
    // !
    // writing to memory may causes an interrupt, to get the the correct return address, the program counter
    // must be incremented before an possible interrupt can occure.
    // !
    acc_dataWrite8bit(rd_addr, result);
}



// ADD - Add without Carry
// 16-bit Opcode: 0000 11rd dddd rrrr
// Adds two registers without the C Flag and places the result in the destination register Rd.
// AVR Instruction Manual page 32
void add() {
    uint16_t instruction = mem_fetchInstruction(cpu_programCounter);
    uint16_t rd_addr = dec_extractBits0000000111110000(instruction);
    uint16_t rr_addr = dec_extractBits0000001000001111(instruction);
    uint8_t rdContent = acc_dataRead8bit(rd_addr);
    uint8_t rrContent = acc_dataRead8bit(rr_addr);
    uint8_t result = rdContent + rrContent;

    bool rdBit3 = uti_getBit(rdContent, 3);
    bool rrBit3 = uti_getBit(rrContent, 3);
    bool resultBit3 = uti_getBit(result, 3);
    bool rdBit7 = uti_getBit(rdContent, 7);
    bool rrBit7 = uti_getBit(rrContent, 7);
    bool resultBit7 = uti_getBit(result, 7);

    // H: Set if there was a carry from bit 3; cleared otherwise.
    reg_sregHalfCarry = (rdBit3 && rrBit3 || rrBit3 && !resultBit3 || !resultBit3 && rdBit3);

    // S = N ⊕ V, for signed tests.
    reg_sregSignBit = (reg_sregNegative ^ reg_sregTwoComplOverflow);

    // V: Set if two’s complement overflow resulted from the operation; cleared otherwise.
    reg_sregTwoComplOverflow = (rdBit7 && rrBit7 && !resultBit7 || !rdBit7 && !rrBit7 && resultBit7);

    // N: Set if MSB of the result is set; cleared otherwise.
    reg_sregNegative = (resultBit7);

    // Z: Set if the result is $00; cleared otherwise.
    reg_sregZero = (result == 0);

    // C: Set if there was carry from the MSB of the result; cleared otherwise.
    reg_sregCarry = (rdBit7 && rrBit7 || rrBit7 && !resultBit7 || !resultBit7 && rdBit7);
    
    cpu_programCounter += 1;
    cpu_incrementCycleCounter(1);
    // !
    // writing to memory may causes an interrupt, to get the the correct return address, the program counter
    // must be incremented before an possible interrupt can occure.
    // !
    acc_dataWrite8bit(rd_addr, result);
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
    uint16_t instruction = mem_fetchInstruction(cpu_programCounter);
    int16_t adressOffset = (int16_t)dec_extractBits0000001111111000(instruction);
    if (!reg_sregZero) {
        cpu_programCounter += (adressOffset - 127) % PROGRAM_MEMORY_END;
        cpu_incrementCycleCounter(2);
    } else {
        cpu_programCounter += 1;
        cpu_incrementCycleCounter(1);
    }
}




// CALL – Long Call to a Subroutine
// 32-bit Opcode: 1001 010k kkkk 111k : kkkk kkkk kkkk kkkk 
// Calls to a subroutine within the entire Program memory. The return address (to the instruction after the
// CALL) will be stored onto the Stack. (See also RCALL). The Stack Pointer uses a post-decrement
// scheme during CALL.
// AVR Instruction Manual page 63
void call() {
    uint16_t jumpDest_addr = mem_fetchInstruction(cpu_programCounter + 1);
    uint16_t stackTop_addr = acc_dataRead16bit(STACKPOINTER);

    acc_dataWrite16bit(stackTop_addr - 1, cpu_programCounter + 2);
    cpu_decrementIncrementStackPointer(-2);
    cpu_programCounter = jumpDest_addr;
    cpu_incrementCycleCounter(4);
}





// CBI – Clear Bit in I/O Register
// 16-bit Opcode: 1001 1000 AAAA Abbb
// Clears a specified bit in an I/O register. This instruction operates on the lower 32 I/O registers –
// addresses 0-31.
// AVR Instruction Manual page 65
void cbi() {
    uint16_t instruction = mem_fetchInstruction(cpu_programCounter);
    uint16_t ioa_addr = dec_extractBits0000000011111000(instruction) + 0x20;
    uint16_t bitNum = dec_extractBits0000000000000111(instruction);
    uint8_t ioaContent = acc_dataRead8bit(ioa_addr);
    uint8_t result = uti_setBit(ioaContent, bitNum, false);

    cpu_programCounter += 1;
    cpu_incrementCycleCounter(1);
    // !
    // writing to memory may causes an interrupt, to get the the correct return address, the program counter
    // must be incremented before an possible interrupt can occure.
    // !
    acc_dataWrite8bit(ioa_addr, result);
}



// CLI – Clear Global Interrupt Flag
// 16-bit Opcode: 1001 0100 1111 1000 
// Clears the Global Interrupt Flag (I) in SREG (Status Register). The interrupts will be immediately
// disabled. No interrupt will be executed after the CLI instruction, even if it occurs simultaneously with the
// CLI instruction.
// AVR Instruction Manual page 69
void cli() {
    reg_sregGlobalInterruptEnable = false;
    cpu_programCounter += 1;
    cpu_incrementCycleCounter(1);
}



// CP - Compare
// 16-bit Opcode: 0001 01rd dddd rrrr
// This instruction performs a compare between two registers Rd and Rr. None of the registers are changed.
// All conditional branches can be used after this instruction.
// AVR Instruction Manual page 77
void cp() {
    uint16_t opCode = mem_fetchInstruction(cpu_programCounter);
    uint16_t rd_addr = dec_extractBits0000000111110000(opCode);
    uint16_t rr_addr = dec_extractBits0000001000001111(opCode);
    uint8_t rdContent = acc_dataRead8bit(rd_addr);
    uint8_t rrContent = acc_dataRead8bit(rr_addr);
    uint8_t result = rdContent - rrContent;

    bool rdBit3 = uti_getBit(rdContent, 3);
    bool rrBit3 = uti_getBit(rrContent, 3);
    bool resultBit3 = uti_getBit(result, 3);
    bool rdBit7 = uti_getBit(rdContent, 7);
    bool rrBit7 = uti_getBit(rrContent, 7);
    bool resultBit7 = uti_getBit(result, 7);

    // H: Set if there was a borrow from bit 3; cleared otherwise.
    reg_sregHalfCarry = (!rdBit3 && rrBit3 || rrBit3 && resultBit3 || resultBit3 && rdBit3);

    // S: N ⊕ V, for signed tests.
    reg_sregSignBit = (reg_sregNegative ^ reg_sregTwoComplOverflow);

    // V: Set if two’s complement overflow resulted from the operation; cleared otherwise.
    reg_sregTwoComplOverflow = (rdBit7 && !rrBit7 && resultBit7 || !rdBit7 && !rrBit7 && resultBit7);

    // N: Set if MSB of the result is set; cleared otherwise.
    reg_sregNegative = (resultBit7);

    // Z: Set if the result is $00; cleared otherwise.
    reg_sregZero = (result == 0);

    // C: Set if the absolute value of the contents of Rr is larger than the absolute value of Rd; cleared
    // otherwise.
    reg_sregCarry = (rrContent > rdContent);

    cpu_programCounter += 1;
    cpu_incrementCycleCounter(1);
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
    uint16_t opCode = mem_fetchInstruction(cpu_programCounter);
    uint16_t rd_addr = dec_extractBits0000000111110000(opCode);
    uint8_t rdContent = acc_dataRead8bit(rd_addr);
    uint8_t result = rdContent - 1;

    bool resultBit7 = uti_getBit(result, 7);

    // S: N ⊕ V, for signed tests.
    reg_sregSignBit = (reg_sregNegative ^ reg_sregTwoComplOverflow);

    // V: Set if two’s complement overflow resulted from the operation; cleared otherwise.
    // Two’s complement overflow occurs if and only if Rd was $80 before the operation.
    reg_sregTwoComplOverflow = (rdContent == 0x80);

    // N: Set if MSB of the result is set; cleared otherwise.
    reg_sregNegative = (resultBit7);

    // Z is set if the result is $00; cleared otherwise.
    reg_sregZero = (result == 0);

    cpu_programCounter += 1;
    cpu_incrementCycleCounter(1);
    // !
    // writing to memory may causes an interrupt, to get the the correct return address, the program counter
    // must be incremented before an possible interrupt can occure.
    // !
    acc_dataWrite8bit(rd_addr, result);
}



// EOR – Exclusive OR
// CLR – Clear Register
// 16-bit Opcode: 0010 01rd dddd rrrr
// Performs the logical EOR between the contents of register Rd and register Rr and places the result in the
// destination register Rd.
// If the both registers are the same this instruction becomes the clr instruction.
// AVR Instruction Manual page 91 and 71
void eorclr() {
    uint16_t opCode = mem_fetchInstruction(cpu_programCounter);
    uint16_t rd_addr = dec_extractBits0000000111110000(opCode);
    uint16_t rr_addr = dec_extractBits0000001000001111(opCode);
    uint8_t rdContent = acc_dataRead8bit(rd_addr);
    uint8_t rrContent = acc_dataRead8bit(rr_addr);
    uint8_t result = rdContent ^ rrContent;

    bool resultBit7 = uti_getBit(result, 7);

    if (rdContent == rrContent) { // it's an clr instruction
        // S: Cleared.
        reg_sregSignBit = (false);

        // V: Cleared.
        reg_sregTwoComplOverflow = (false);

        // N: Cleared.
        reg_sregNegative = (false);

        // Z: Set.
        reg_sregZero = (true);

    } else { // it's an eor instruction
        // S: N ⊕ V, for signed tests.
        reg_sregSignBit = (reg_sregNegative ^ reg_sregTwoComplOverflow);

        // V: Cleared.
        reg_sregTwoComplOverflow = (false);

        // N: Set if MSB of the result is set; cleared otherwise.
        reg_sregNegative = (resultBit7);

        // Z is set if the result is $00; cleared otherwise.
        reg_sregZero = (result == 0);
    }

    cpu_programCounter += 1;
    cpu_incrementCycleCounter(1);
    // !
    // writing to memory may causes an interrupt, to get the the correct return address, the program counter
    // must be incremented before an possible interrupt can occure.
    // !
    acc_dataWrite8bit(rd_addr, result);
}




// IN - Load an I/O Location to Register
// 16-bit Opcode: 1011 0AAd dddd AAAA
// Loads data from the I/O Space (Ports, Timers, Configuration Registers, etc.) into register Rd in the
// Register File.
// AVR Instruction Manual page 100
void in() {
    uint16_t opCode = mem_fetchInstruction(cpu_programCounter);
    uint16_t ioa_addr = dec_extractBits0000011000001111(opCode) + 0x20;
    uint16_t rd_addr =  dec_extractBits0000000111110000(opCode);
    uint8_t ioaContent = acc_dataRead8bit(ioa_addr);

    cpu_programCounter += 1;
    cpu_incrementCycleCounter(1);
    // !
    // writing to memory may causes an interrupt, to get the the correct return address, the program counter
    // must be incremented before an possible interrupt can occure.
    // !
    acc_dataWrite8bit(rd_addr, ioaContent);
}




// JMP – Jump
// 32-bit Opcode: 1001 010k kkkk 110k kkkk kkkk kkkk kkkk
// Jump to an address within the entire 4M (words) Program memory.
// AVR Instruction Manual page 103
void jmp() {
    uint16_t jumpDest_addr = mem_fetchInstruction(cpu_programCounter + 1);

    cpu_programCounter = jumpDest_addr;
    cpu_incrementCycleCounter(1);
}



// LDI – Load Immediate
// 16-bit Opcode: 1110 KKKK dddd KKKK
// Loads an 8-bit constant directly to register 16 to 31.
// AVR Instruction Manual page 115
void ldi() {
    uint16_t opCode = mem_fetchInstruction(cpu_programCounter);
    uint16_t rd_addr =  dec_extractBits0000000011110000(opCode) + 16;
    uint8_t constData = dec_extractBits0000111100001111(opCode);

    cpu_programCounter += 1;
    cpu_incrementCycleCounter(1);
    // !
    // writing to memory may causes an interrupt, to get the the correct return address, the program counter
    // must be incremented before an possible interrupt can occure.
    // !
    acc_dataWrite8bit(rd_addr, constData);
}




// LDS – Load Direct from Data Space
// 32-bit Opcode: 1001 000d dddd 0000 kkkk kkkk kkkk kkkk
// Loads one byte from the data space to a register.
// AVR Instruction Manual page 116
void lds32() {
    uint16_t opCode = mem_fetchInstruction(cpu_programCounter);
    uint16_t rd_addr =  dec_extractBits0000000111110000(opCode);
    uint8_t constAddress = mem_fetchInstruction(cpu_programCounter + 1); 
    uint8_t memContent = acc_dataRead8bit(constAddress);

    cpu_programCounter += 2;
    cpu_incrementCycleCounter(2);
    // !
    // writing to memory may causes an interrupt, to get the the correct return address, the program counter
    // must be incremented before an possible interrupt can occure.
    // !
    acc_dataWrite8bit(rd_addr, memContent);
}




// LDS (16-bit) – Load Direct from Data Space
// 16-bit Opcode: 1010 0kkk dddd kkkk
// Loads one byte from the data space to a register. 
// A 7-bit address must be supplied. The address given in the instruction is coded to a data space address
// as follows:
// ADDR[7:0] = (!INST[8], INST[8], INST[10], INST[9], INST[3], INST[2], INST[1], INST[0])
// Memory access is limited to the address range 0x40...0xbf.
// Note: Registers r0...r15 are remapped to r16...r31.
// AVR Instruction Manual page 117
void lds16() {
    uint16_t opCode = mem_fetchInstruction(cpu_programCounter);
    uint16_t rd_addr =  dec_extractBits0000000011110000(opCode);

    uint8_t opCodeBit0 = uti_getBit(opCode, 0);
    uint8_t opCodeBit1 = uti_getBit(opCode, 1);
    uint8_t opCodeBit2 = uti_getBit(opCode, 2);
    uint8_t opCodeBit3 = uti_getBit(opCode, 3);
    uint8_t opCodeBit8 = uti_getBit(opCode, 8);
    uint8_t opCodeBit9 = uti_getBit(opCode, 9);
    uint8_t opCodeBit10 = uti_getBit(opCode, 10);

    uint16_t constAddress = 0;

    constAddress = uti_setBitInWord(constAddress, opCodeBit0, 0);
    constAddress = uti_setBitInWord(constAddress, opCodeBit1, 1);
    constAddress = uti_setBitInWord(constAddress, opCodeBit2, 2);
    constAddress = uti_setBitInWord(constAddress, opCodeBit3, 3);
    constAddress = uti_setBitInWord(constAddress, opCodeBit9, 4);
    constAddress = uti_setBitInWord(constAddress, opCodeBit10, 5);
    constAddress = uti_setBitInWord(constAddress, opCodeBit8, 6);
    constAddress = uti_setBitInWord(constAddress, !opCodeBit8, 7);
//TODO check th address offset 
    if (rd_addr < 32) rd_addr %= 32; // Registers r0...r15 are remapped to r16...r31.

    uint8_t memContent = mem_eepromRead8bit(constAddress);

    cpu_programCounter += 1;
    cpu_incrementCycleCounter(1);
    // !
    // writing to memory may causes an interrupt, to get the the correct return address, the program counter
    // must be incremented before an possible interrupt can occure.
    // !
    acc_dataWrite8bit(rd_addr, memContent);
}




// NOP - No Operation
// 16-bit Opcode: 0000 0000 0000 0000
// This instruction performs a single cycle No Operation.
// AVR Instruction Manual page 131
void nop() {
    uint16_t opCode = mem_fetchInstruction(cpu_programCounter);

    cpu_programCounter += 1;
    cpu_incrementCycleCounter(1);
}




// ORI – Logical OR with Immediate
// 16-bit Opcode: 0110 KKKK dddd KKKK
// Performs the logical OR between the contents of register Rd and a constant, and places the result in the
// destination register Rd.
// AVR Instruction Manual page 133
void ori() {
    uint16_t opCode = mem_fetchInstruction(cpu_programCounter);
    uint16_t rd_addr =  dec_extractBits0000000011110000(opCode) + 16;
    uint8_t constData = dec_extractBits0000111100001111(opCode);
    uint8_t rdContent = acc_dataRead8bit(rd_addr);
    uint8_t result = rdContent | constData;
    uint8_t resultBit7 = uti_getBit(result, 7);

    // S: N ⊕ V, for signed tests.
    reg_sregSignBit = (reg_sregNegative ^ reg_sregTwoComplOverflow);

    // V: Cleared.
    reg_sregTwoComplOverflow = (false);

    // N: Set if MSB of the result is set; cleared otherwise.
    reg_sregNegative = (resultBit7);

    // Z: Set if the result is $00; cleared otherwise.
    reg_sregZero = (result == 0);

    cpu_programCounter += 1;
    cpu_incrementCycleCounter(1);
    // !
    // writing to memory may causes an interrupt, to get the the correct return address, the program counter
    // must be incremented before an possible interrupt can occure.
    // !
    acc_dataWrite8bit(rd_addr, result);
}    




// OUT – Store Register to I/O Location
// 16-bit Opcode: 1011 1AAr rrrr AAAA
// Stores data from register Rr in the Register File to I/O Space (Ports, Timers, Configuration Registers, etc.).
// When using the I/O specific commands IN and OUT, the I/O addresses 0x00 - 0x3F must be used.
// That means 0x20 must bei added to the provided adresses.
// Atmega328 datasheet page 624
// AVR Instruction Manual page 134
void out() {
    uint16_t opCode = mem_fetchInstruction(cpu_programCounter);
    uint16_t ioa_addr = dec_extractBits0000011000001111(opCode) + 0x20;
    uint16_t rr_addr =  dec_extractBits0000000111110000(opCode);
    uint8_t rrContent = acc_dataRead8bit(rr_addr);

    cpu_programCounter += 1;
    // !
    // writing to memory may causes an interrupt, to get the the correct return address, the program counter
    // must be incremented before an possible interrupt can occure.
    // !
    acc_dataWrite8bit(ioa_addr, rrContent);
    cpu_incrementCycleCounter(1);
}




// POP - Pop Register from Stack
// 16-bit Opcode: 1001 000d dddd 1111
// This instruction loads register Rd with a byte from the STACK. The Stack Pointer is pre-incremented by 1
// before the POP.
// This instruction is not available in all devices. Refer to the device specific instruction set summary.
// AVR Instruction Manual page 135
void pop() {
    uint16_t opCode = mem_fetchInstruction(cpu_programCounter);
    uint16_t rd_addr = dec_extractBits0000000111110000(opCode);
    uint16_t stackPointer = acc_dataRead16bit(STACKPOINTER);
    uint8_t stackContent = acc_dataRead16bit(stackPointer + 1);

    acc_dataWrite8bit(rd_addr, stackContent);
    acc_dataWrite16bit(STACKPOINTER, stackPointer + 1);
    cpu_programCounter += 1;
    cpu_incrementCycleCounter(2);
}




// PUSH – Push Register on Stack
// 16-bit Opcode: 1001 001d dddd 1111
// This instruction stores the contents of register Rr on the STACK. The Stack Pointer is post-decremented
// by 1 after the PUSH.
// This instruction is not available in all devices. Refer to the device specific instruction set summary.
// AVR Instruction Manual page 136
void push() {
    uint16_t opCode = mem_fetchInstruction(cpu_programCounter);
    uint16_t rr_addr = dec_extractBits0000000111110000(opCode);
    uint16_t stackPointer = acc_dataRead16bit(STACKPOINTER);
    uint8_t rrContent = acc_dataRead8bit(rr_addr);

    acc_dataWrite8bit(stackPointer, rrContent);
    acc_dataWrite16bit(STACKPOINTER, stackPointer - 1);
    cpu_programCounter += 1;
    cpu_incrementCycleCounter(1);
}




// RCALL – Relative Call to Subroutine
// 16-bit Opcode: 1101 kkkk kkkk kkkk
// Relative call to an address within PC - 2K + 1 and PC + 2K (words). The return address (the instruction
// after the RCALL) is stored onto the Stack. See also CALL. For AVR microcontrollers with Program
// memory not exceeding 4K words (8KB) this instruction can address the entire memory from every
// address location. The Stack Pointer uses a post-decrement scheme during RCALL.
// AVR Instruction Manual page 137
#pragma GCC push_options
#pragma GCC optimize ("O1") 
void rcall() {
    uint16_t opCode = mem_fetchInstruction(cpu_programCounter);
    uint16_t stackTop_addr = acc_dataRead16bit(STACKPOINTER);
    uint16_t addressOffset = (uint16_t)dec_extractBits0000011111111111(opCode);
    bool signBit = uti_getBit(opCode, 11);
    uint16_t jumpDest_addr;
    if(signBit) {
        jumpDest_addr = (PROGRAM_MEMORY_END + 2 + cpu_programCounter + addressOffset - 0x800)  % (PROGRAM_MEMORY_END + 1);
    } else {
        jumpDest_addr = (PROGRAM_MEMORY_END + 2 + cpu_programCounter + addressOffset)  % (PROGRAM_MEMORY_END + 1);
    }

    cpu_programCounter = jumpDest_addr;
    acc_dataWrite16bit(stackTop_addr - 1, cpu_programCounter + 1);
    cpu_decrementIncrementStackPointer(-2);
    cpu_incrementCycleCounter(3);
}
#pragma GCC pop_options



// RET – Return from Subroutine
// 16-bit Opcode: 1001 0101 0000 1000
// Returns from subroutine. The return address is loaded from the STACK. The Stack Pointer uses a pre-
// increment scheme during RET.
// AVR Instruction Manual page 139
void ret() {
    uint16_t stackTop_addr = acc_dataRead16bit(STACKPOINTER);
    uint16_t jumpDest_addr =  acc_dataRead16bit(stackTop_addr + 2);

    cpu_decrementIncrementStackPointer(2);
    cpu_programCounter = jumpDest_addr;
    cpu_incrementCycleCounter(4);
}




// RETI – Return from Interrupt
// 16-bit Opcode: 1001 0101 0001 1000
// Returns from interrupt. The return address is loaded from the STACK and the Global Interrupt Flag is set.
// Note that the Status Register is not automatically stored when entering an interrupt routine, and it is not
// restored when returning from an interrupt routine. This must be handled by the application program. The
// Stack Pointer uses a pre-increment scheme during RETI.
// AVR Instruction Manual page 140
void reti() {
    uint16_t stackTop_addr = acc_dataRead16bit(STACKPOINTER);
    uint16_t jumpDest_addr =  acc_dataRead16bit(stackTop_addr + 2);

    reg_sregGlobalInterruptEnable = true;
    cpu_decrementIncrementStackPointer(2);
    cpu_programCounter = jumpDest_addr;
    cpu_incrementCycleCounter(4);
}




// RJMP – Relative Jump
// 16-bit Opcode: 1100 kkkk kkkk kkkk
// Relative jump to an address within PC - 2K +1 and PC + 2K (words). For AVR microcontrollers with
// Program memory not exceeding 4K words (8KB) this instruction can address the entire memory from
// every address location. See also JMP.
// AVR Instruction Manual page 142
#pragma GCC push_options
#pragma GCC optimize ("O1") 
void rjmp() {
    uint16_t opCode = mem_fetchInstruction(cpu_programCounter);
    uint16_t addressOffset = dec_extractBits0000011111111111(opCode);
    bool signBit = uti_getBit(opCode, 11);
    uint16_t jumpDest_addr;
    if (signBit) {
        jumpDest_addr = (PROGRAM_MEMORY_END + 2 + cpu_programCounter + addressOffset - 0x800) % (PROGRAM_MEMORY_END + 1);
    } else {
        jumpDest_addr = (PROGRAM_MEMORY_END + 2 + cpu_programCounter + addressOffset) % (PROGRAM_MEMORY_END + 1);
    }
    
    cpu_programCounter = jumpDest_addr;
    cpu_incrementCycleCounter(1);
}
#pragma GCC pop_options  




// SBCI – Subtract Immediate with Carry SBI – Set Bit in I/O Register
// 16-bit Opcode: 0100 KKKK dddd KKKK
// Subtracts a constant from a register and subtracts with the C Flag, and places the result in the destination
// register Rd.
// AVR Instruction Manual page 149
void sbci() {
    uint16_t opCode = mem_fetchInstruction(cpu_programCounter);
    uint8_t rd_addr = dec_extractBits0000000111110000(opCode) + 16;
    uint8_t rdContent = acc_dataRead8bit(rd_addr);
    uint8_t constData = dec_extractBits0000111100001111(opCode);
    uint8_t result = rdContent - constData - (uint8_t)reg_sregCarry;

    bool rdBit3 = uti_getBit(rdContent, 3);
    bool constDataBit3 = uti_getBit(constData, 3);
    bool resultBit3 = uti_getBit(result, 3);
    bool rdBit7 = uti_getBit(rdContent, 7);
    bool constDataBit7 = uti_getBit(constData, 7);
    bool resultBit7 = uti_getBit(result, 7);


    // H: Set if there was a borrow from bit 3; cleared otherwise.
    reg_sregHalfCarry = (!rdBit3 && constDataBit3 || constDataBit3 && resultBit3 || resultBit3 && !rdBit3);

    // S: N ⊕ V, for signed tests.
    reg_sregSignBit = (reg_sregNegative ^ reg_sregTwoComplOverflow);

    // V: Set if two’s complement overflow resulted from the operation; cleared otherwise.
    reg_sregTwoComplOverflow = (rdBit7 && !constDataBit7 && !resultBit7 || !rdBit7 && constDataBit7 && resultBit7);

    // N: Set if MSB of the result is set; cleared otherwise.
    reg_sregNegative = (resultBit7);

    // Z: Previous value remains unchanged when the result is zero; cleared otherwise.
    if(result != 0) reg_sregZero = (false);

    // C: Set if the absolute value of the constant plus previous carry is larger than the absolute value of Rd;
    // cleared otherwise.
    reg_sregCarry = (constData + (uint8_t)reg_sregCarry > rdContent);

    cpu_programCounter += 1;
    cpu_incrementCycleCounter(1);
    // !
    // writing to memory may causes an interrupt, to get the the correct return address, the program counter
    // must be incremented before an possible interrupt can occure.
    // !
    acc_dataWrite8bit(rd_addr, result);
}





// SBI – Set Bit in I/O Register
// 16-bit Opcode: 1001 1010 AAAA Abbb
// Sets a specified bit in an I/O Register. This instruction operates on the lower 32 I/O Registers –
// addresses 0-31.
// AVR Instruction Manual page 151
void sbi() {
    uint16_t opCode = mem_fetchInstruction(cpu_programCounter);
    uint16_t ioa_addr = dec_extractBits0000000011111000(opCode) + 0x20;
    uint16_t bitNum = dec_extractBits0000000000000111(opCode);
    uint8_t ioaContent = acc_dataRead8bit(ioa_addr);
    uint8_t result = uti_setBit(ioaContent, bitNum, true);

    cpu_programCounter += 1;
    cpu_incrementCycleCounter(1);
    // !
    // writing to memory may causes an interrupt, to get the the correct return address, the program counter
    // must be incremented before an possible interrupt can occure.
    // !
    acc_dataWrite8bit(ioa_addr, result);
}




// SBIC – Skip if Bit in I/O Register is Cleared
// 16-bit Opcode: 1001 1001 AAAA Abbb
// This instruction tests a single bit in an I/O Register and skips the next instruction if the bit is cleared. This
// instruction operates on the lower 32 I/O Registers – addresses 0-31.
// AVR Instruction Manual page 152
void sbic() {
    uint16_t opCode = mem_fetchInstruction(cpu_programCounter);
    uint16_t nextOpCode = mem_fetchInstruction(cpu_programCounter + 1);
    uint16_t ioa_addr = dec_extractBits0000000001111000(opCode) + 0x20;
    uint16_t bitNum = dec_extractBits0000000000000111(opCode);
    uint8_t ioaContent = acc_dataRead8bit(ioa_addr);

    #define CALL_INSTRUCTION_CODE 0b1001010000001110
    #define CALL_INSTRUCTION_MASK 0b1111111000001110

    #define JUMP_INSTRUCTION_CODE 0b1001010000001100
    #define JUMP_INSTRUCTION_MASK 0b1111111000001110

    #define LDS_INSTRUCTION_CODE 0b1001000000000000
    #define LDS_INSTRUCTION_MASK 0b1111111000001111

    #define STS_INSTRUCTION_CODE 0b1001001000000000
    #define STS_INSTRUCTION_MASK 0b1111111000001111


    if (uti_getBit(ioaContent, bitNum)) {
        cpu_programCounter += 1;
        cpu_incrementCycleCounter(1);
    } else if ((nextOpCode & CALL_INSTRUCTION_MASK) == CALL_INSTRUCTION_CODE) {
        cpu_programCounter += 3;
        cpu_incrementCycleCounter(3);
    } else if ((nextOpCode & JUMP_INSTRUCTION_MASK) == JUMP_INSTRUCTION_CODE) {
        cpu_programCounter += 3;
        cpu_incrementCycleCounter(3);
    } else if ((nextOpCode & LDS_INSTRUCTION_MASK) == LDS_INSTRUCTION_CODE) {
        cpu_programCounter += 3;
        cpu_incrementCycleCounter(3);
    } else if ((nextOpCode & STS_INSTRUCTION_MASK) == STS_INSTRUCTION_CODE) {
        cpu_programCounter += 3;
        cpu_incrementCycleCounter(3);
    } else {
        cpu_programCounter += 2;
        cpu_incrementCycleCounter(2);
    }
}




// SBIS – Skip if Bit in I/O Register is Set
// 16-bit Opcode: 1001 1011 AAAA Abbb
// This instruction tests a single bit in an I/O Register and skips the next instruction if the bit is set. This
// instruction operates on the lower 32 I/O Registers – addresses 0-31.
// AVR Instruction Manual page 153
void sbis() {
    uint16_t opCode = mem_fetchInstruction(cpu_programCounter);
    uint16_t nextOpCode = mem_fetchInstruction(cpu_programCounter + 1);
    uint16_t ioa_addr = dec_extractBits0000000001111000(opCode) + 0x20;
    uint16_t bitNum = dec_extractBits0000000000000111(opCode);
    uint8_t ioaContent = acc_dataRead8bit(ioa_addr);

    #define CALL_INSTRUCTION_CODE 0b1001010000001110
    #define CALL_INSTRUCTION_MASK 0b1111111000001110

    #define JUMP_INSTRUCTION_CODE 0b1001010000001100
    #define JUMP_INSTRUCTION_MASK 0b1111111000001110

    #define LDS_INSTRUCTION_CODE 0b1001000000000000
    #define LDS_INSTRUCTION_MASK 0b1111111000001111

    #define STS_INSTRUCTION_CODE 0b1001001000000000
    #define STS_INSTRUCTION_MASK 0b1111111000001111


    if (!uti_getBit(ioaContent, bitNum)) {
        cpu_programCounter += 1;
        cpu_incrementCycleCounter(1);
    } else if ((nextOpCode & CALL_INSTRUCTION_MASK) == CALL_INSTRUCTION_CODE) {
        cpu_programCounter += 3;
        cpu_incrementCycleCounter(3);
    } else if ((nextOpCode & JUMP_INSTRUCTION_MASK) == JUMP_INSTRUCTION_CODE) {
        cpu_programCounter += 3;
        cpu_incrementCycleCounter(3);
    } else if ((nextOpCode & LDS_INSTRUCTION_MASK) == LDS_INSTRUCTION_CODE) {
        cpu_programCounter += 3;
        cpu_incrementCycleCounter(3);
    } else if ((nextOpCode & STS_INSTRUCTION_MASK) == STS_INSTRUCTION_CODE) {
        cpu_programCounter += 3;
        cpu_incrementCycleCounter(3);
    } else {
        cpu_programCounter += 2;
        cpu_incrementCycleCounter(2);
    }
}




// SBIW – Subtract Immediate from Word
// 16-bit Opcode: 1001 0111 KKdd KKKK
// Subtracts an immediate value (0-63) from a register pair and places the result in the register pair.
// This instruction operates on the upper four register pairs, and is well suited for operations on the
// Pointer Registers.
// AVR Instruction Manual page 154
void sbiw() {
    uint16_t instruction = mem_fetchInstruction(cpu_programCounter);
    uint16_t rd_addr =   dec_extractBits0000000000110000(instruction) * 2 + R24;
    uint16_t constData = dec_extractBits0000000011001111(instruction);
    uint16_t rdContent = acc_dataRead16bit(rd_addr);
    uint16_t result = rdContent - constData;

    bool resultBit15 = uti_getBit(result, 15);
    bool rdhBit7 = uti_getBit(rdContent, 7);

    // S = N ⊕ V, for signed tests.
    reg_sregSignBit = (reg_sregNegative ^ reg_sregTwoComplOverflow);

    // V: Set if two’s complement overflow resulted from the operation; cleared otherwise.
    reg_sregTwoComplOverflow = (resultBit15 && !rdhBit7);

    // N: Set if MSB of the result is set; cleared otherwise.
    reg_sregNegative = (resultBit15);

    // Z: Set if the result is $0000; cleared otherwise.
    reg_sregZero = (result == 0);

    // C: Set if the absolute value of K is larger than the absolute value of Rd; cleared otherwise.
    reg_sregSignBit = (resultBit15 && rdhBit7);

    cpu_programCounter += 1;
    cpu_incrementCycleCounter(2);
    // !
    // writing to memory may causes an interrupt, to get the the correct return address, the program counter
    // must be incremented before an possible interrupt can occure.
    // !
    acc_dataWrite16bit(rd_addr, result);
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
// AVR Instruction Manual page 179
void sts() {
    uint16_t instructionFirst = mem_fetchInstruction(cpu_programCounter);
    uint16_t rr_addr = dec_extractBits0000000111110000(instructionFirst);
    uint16_t instructionSecond = mem_fetchInstruction(cpu_programCounter + 1);
    uint16_t mem_addr = instructionSecond;
    uint8_t memContent = acc_dataRead8bit(rr_addr);

    cpu_programCounter += 2;
    cpu_incrementCycleCounter(1);
    // !
    // writing to memory may causes an interrupt, to get the the correct return address, the program counter
    // must be incremented before an possible interrupt can occure.
    // !
    acc_dataWrite8bit(mem_addr, memContent);
}




// SUBI – Subtract Immediate
// 16-bit Opcode: 0101 KKKK dddd KKKK
// Subtracts a register and a constant, and places the result in the destination register Rd. This instruction is
// working on Register R16 to R31 and is very well suited for operations on the X, Y, and Z-pointers.
// AVR Instruction Manual page 183
void subi() {
    uint16_t opCode = mem_fetchInstruction(cpu_programCounter);
    uint8_t rd_addr = dec_extractBits0000000111110000(opCode) + 16;
    uint8_t rdContent = acc_dataRead8bit(rd_addr);
    uint8_t constData = dec_extractBits0000111100001111(opCode);
    uint8_t result = rdContent - constData;

    bool rdBit3 = uti_getBit(rdContent, 3);
    bool constDataBit3 = uti_getBit(constData, 3);
    bool resultBit3 = uti_getBit(result, 3);
    bool rdBit7 = uti_getBit(rdContent, 7);
    bool constDataBit7 = uti_getBit(constData, 7);
    bool resultBit7 = uti_getBit(result, 7);

    // H: Set if there was a borrow from bit 3; cleared otherwise.
    reg_sregHalfCarry = (!rdBit3 && constDataBit3 || constDataBit3 && resultBit3 || resultBit3 && !rdBit3);

    // S: N ⊕ V, for signed tests.
    reg_sregSignBit = (reg_sregNegative ^ reg_sregTwoComplOverflow);

    // V: Set if two’s complement overflow resulted from the operation; cleared otherwise.
    reg_sregTwoComplOverflow = (rdBit7 && !constDataBit7 && !resultBit7 || !rdBit7 && constDataBit7 && resultBit7);

    // N: Set if MSB of the result is set; cleared otherwise.
    reg_sregNegative = (resultBit7);

    // Z: Set if the result is $00; cleared otherwise.
    reg_sregZero = (result == 0);

    // C: Set if the absolute value of K is larger than the absolute value of Rd; cleared otherwise.
    reg_sregCarry = (constData > rdContent);

    cpu_programCounter += 1;
    cpu_incrementCycleCounter(1);
    // !
    // writing to memory may causes an interrupt, to get the the correct return address, the program counter
    // must be incremented before an possible interrupt can occure.
    // !
    acc_dataWrite8bit(rd_addr, result);
}




