#pragma once

#include "decode.h"


#define INFO_LENGTH 80

typedef struct InstructionInfo {
    uint8_t length;
    char info[INFO_LENGTH];
} InstructionInfo;



// unknown opcode
InstructionInfo unknown_disassemble(uint16_t opCode) {
    char *instructionName = "unknown";
    
    InstructionInfo instruction = {0};

    snprintf(instruction.info, INFO_LENGTH, "unknown %04X", opCode);

    instruction.length =  1;
    
    return instruction;
}



// ADC - Add with Carry
// 16-bit Opcode: 0001 11rd dddd rrrr
// Adds two registers and the contents of the C Flag and places the result in the destination register Rd.
// AVR Instruction Manual page 30
InstructionInfo adc_disassemble(uint16_t opCode) {
    uint16_t rd_addr = dec_extractBits0000000111110000(opCode);
    uint16_t rr_addr = dec_extractBits0000001000001111(opCode);

    InstructionInfo instruction = {0};

    snprintf(instruction.info, INFO_LENGTH, "adc %s %s", deb_getName(rd_addr), deb_getName(rr_addr));

    instruction.length =  1;
    
    return instruction;
}



// ADD - Add without Carry
// 16-bit Opcode: 0000 11rd dddd rrrr
// Adds two registers without the C Flag and places the result in the destination register Rd.
// AVR Instruction Manual page 32
InstructionInfo add_disassemble(uint16_t opCode) {
    char *instructionName = "add";
    uint16_t rd_addr = dec_extractBits0000000111110000(opCode);
    uint16_t rr_addr = dec_extractBits0000001000001111(opCode);

    InstructionInfo instruction = {0};

    snprintf(instruction.info, INFO_LENGTH, "add %s %s", deb_getName(rd_addr), deb_getName(rr_addr));

    instruction.length =  1;
    
    return instruction;
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
InstructionInfo brne_disassemble(uint16_t opCode) {
    char *instructionName = "brne";
    int16_t constData = (int16_t)dec_extractBits0000001111111000(opCode);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "brne %04X", mem_programCounter + constData - 128);
    
    instruction.length =  1;
    
    return instruction;
}



// CBI – Clear Bit in I/O Register
// 16-bit Opcode: 1001 1000 AAAA Abbb
// Clears a specified bit in an I/O register. This instruction operates on the lower 32 I/O registers –
// addresses 0-31.
// AVR Instruction Manual page 65
InstructionInfo cbi_disassemble(uint16_t opCode) {
    uint16_t ioa_addr = dec_extractBits0000000001111000(opCode) + 0x20;
    uint16_t bitNum = dec_extractBits0000000000000111(opCode);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "cbi %s %1X", deb_getName(ioa_addr), bitNum);

    instruction.length =  1;
    
    return instruction;
}    



// CP - Compare
// 16-bit Opcode: 0001 01rd dddd rrrr
// This instruction performs a compare between two registers Rd and Rr. None of the registers are changed.
// All conditional branches can be used after this instruction.
// AVR Instruction Manual page 77
InstructionInfo cp_disassemble() {
    uint16_t instructionFirst = mem_programFetchInstruction(mem_programCounter);
    uint16_t rd_addr = dec_extractBits0000000111110000(instructionFirst);
    uint16_t rr_addr = dec_extractBits0000001000001111(instructionFirst);
    uint8_t rdContent = mem_dataRead8bitFromCPU(rd_addr);
    uint8_t rrContent = mem_dataRead8bitFromCPU(rr_addr);
    uint8_t result = rdContent - rrContent;

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "cp \"%s\"(%02X) \"%s\"(%02X)", deb_getName(rd_addr), rdContent, deb_getName(rr_addr), rrContent);

    instruction.length =  1;
    
    return instruction;
}




// DEC – Decrement
// 16-bit Opcode: 1001 010d dddd 1010
// Subtracts one -1- from the contents of register Rd and places the result in the destination register Rd.
// The C Flag in SREG is not affected by the operation, thus allowing the DEC instruction to be used on a
// loop counter in multiple-precision computations.
// When operating on unsigned values, only BREQ and BRNE branches can be expected to perform
// consistently. When operating on two’s complement values, all signed branches are available.
// AVR Instruction Manual page 84
InstructionInfo dec_disassemble(uint16_t opCode) {
    char *instructionName = "dec";
    uint16_t rd_addr = dec_extractBits0000000111110000(opCode);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "dec %s", instructionName, deb_getName(rd_addr));

    instruction.length =  1;
    
    return instruction;
}




// EOR – Exclusive OR
// CLR – Clear Register
// 16-bit Opcode: 0010 01rd dddd rrrr
// Performs the logical EOR between the contents of register Rd and register Rr and places the result in the
// destination register Rd.
// If the both registers are the same this instruction becomes the clr instruction.
// AVR Instruction Manual page 91 and 71
InstructionInfo eor_disassemble(uint16_t opCode) {
    char *instructionName = "eor";
    uint16_t rd_addr = dec_extractBits0000000111110000(opCode);
    uint16_t rr_addr = dec_extractBits0000001000001111(opCode);

    InstructionInfo instruction = {0};
 
    if (rd_addr == rr_addr) {
        snprintf(instruction.info, INFO_LENGTH, "eor %s %s", deb_getName(rd_addr), deb_getName(rr_addr));
    } else {
        snprintf(instruction.info, INFO_LENGTH, "clr %s", deb_getName(rd_addr));
    }

    instruction.length =  1;
    
    return instruction;
}


InstructionInfo in_disassemble(uint16_t opCode) {
    char *instructionName = "in";
    uint16_t ioa_addr = dec_extractBits0000011000001111(opCode) + 0x20;
    uint16_t rr_addr =  dec_extractBits0000000111110000(opCode);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "%s %s %s", instructionName, deb_getName(rr_addr), deb_getName(ioa_addr));

    instruction.length =  1;
    
    return instruction;
}    


InstructionInfo ldi_disassemble(uint16_t opCode) {
    uint16_t rd_addr =  dec_extractBits0000000011110000(opCode) + 16;
    uint8_t constData = dec_extractBits0000111100001111(opCode);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "ldi \"%s\" (%02X)", deb_getName(rd_addr), constData);

    instruction.length =  1;
    
    return instruction;
}


InstructionInfo nop_disassemble(uint16_t opCode) {
    char *instructionName = "nop";

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "%s", instructionName);

    instruction.length =  1;
    
    return instruction;
}


InstructionInfo out_disassemble(uint16_t opCode) {
    char *instructionName = "out";
    uint16_t ioa_addr = dec_extractBits0000011000001111(opCode) + 0x20;
    uint16_t rr_addr =  dec_extractBits0000000111110000(opCode);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "%s %s %s", instructionName, deb_getName(ioa_addr), deb_getName(rr_addr));

    instruction.length =  1;
    
    return instruction;
}



// SBIC – Skip if Bit in I/O Register is Cleared
// This instruction tests a single bit in an I/O Register and skips the next instruction if the bit is cleared. This
// instruction operates on the lower 32 I/O Registers – addresses 0-31.
// AVR Instruction Manual page 152
InstructionInfo sbic_disassemble(uint16_t opCode) {
    uint16_t ioa_addr = dec_extractBits0000000001111000(opCode) + 0x20;
    uint16_t bitNum = dec_extractBits0000000000000111(opCode);
    uint8_t ioaContent = mem_dataRead8bitFromCPU(ioa_addr);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "sbic %s", deb_getName(ioa_addr));

    instruction.length =  1;
    
    return instruction;
}



// SBIS – Skip if Bit in I/O Register is Set
// This instruction tests a single bit in an I/O Register and skips the next instruction if the bit is set. This
// instruction operates on the lower 32 I/O Registers – addresses 0-31.
// AVR Instruction Manual page 153
InstructionInfo sbis_disassemble(uint16_t opCode) {
    uint16_t ioa_addr = dec_extractBits0000000001111000(opCode) + 0x20;
    uint16_t bitNum = dec_extractBits0000000000000111(opCode);
    uint8_t ioaContent = mem_dataRead8bitFromCPU(ioa_addr);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "sbis %s", deb_getName(ioa_addr));

    instruction.length =  1;
    
    return instruction;
}




// SBIW – Subtract Immediate from Word
// 16-bit Opcode: 1001 0111 KKdd KKKK
// Subtracts an immediate value (0-63) from a register pair and places the result in the register pair.
// This instruction operates on the upper four register pairs, and is well suited for operations on the
// Pointer Registers.
// AVR Instruction Manual page 154
InstructionInfo sbiw_disassemble(uint16_t opCode) {
    uint16_t rd_addr =   dec_extractBits0000000000110000(opCode) * 2 + R24;
    uint16_t constData = dec_extractBits0000000011001111(opCode);
    uint16_t rdContent = mem_dataMemoryRead16bit(rd_addr);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "sbiw \"%s\"(%04X) %04X", deb_getName(rd_addr), rdContent, constData);

    instruction.length =  1;
    
    return instruction;
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
InstructionInfo sts_disassemble(uint16_t opCode) {
    uint16_t instructionFirst = mem_programFetchInstruction(mem_programCounter);
    uint16_t rr_addr = dec_extractBits0000000111110000(instructionFirst);
    uint16_t instructionSecond = mem_programFetchInstruction(mem_programCounter + 1);
    uint16_t mem_addr = instructionSecond;
    uint8_t rrContent = mem_dataRead8bitFromCPU(rr_addr);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "sts %04X \"%s\"(%02X)", mem_addr, deb_getName(rr_addr), rrContent);
    
    instruction.length =  2;

    return instruction;
}




InstructionInfo rjmp_disassemble(uint16_t opCode) {
    int16_t constAddress = (int16_t)dec_extractBits0000111111111111(opCode);
    uint16_t jumpDest_addr = (mem_programCounter + constAddress - 0xfff) % (DATA_MEMORY_END + 1);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "rjmp %04X", jumpDest_addr);

    instruction.length =  1;

    return instruction;
}
