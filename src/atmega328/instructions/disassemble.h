#pragma once

#include "decoder.h"


#define INFO_LENGTH 80

typedef struct InstructionInfo {
    uint8_t length;
    char info[INFO_LENGTH];
    char comment[INFO_LENGTH];
} InstructionInfo;



static char *_getName(int address) {
    switch (address) {
        case R0: return "R0";
        case R1: return "R1";
        case R2: return "R2";
        case R3: return "R3";
        case R4: return "R4";
        case R5: return "R5";
        case R6: return "R6";
        case R7: return "R7";
        case R8: return "R8";
        case R9: return "R9";
        case R10: return "R10";
        case R11: return "R11";
        case R12: return "R12";
        case R13: return "R13";
        case R14: return "R14";
        case R15: return "R15";
        case R16: return "R16";
        case R17: return "R17";
        case R18: return "R18";
        case R19: return "R19";
        case R20: return "R20";
        case R21: return "R21";
        case R22: return "R22";
        case R23: return "R23";
        case R24: return "R24";
        case R25: return "R25";
        case R26_X_REGISTER_LOW_BYTE: return "R26";
        case R27_X_REGISTER_HIGH_BYTE: return "R27";
        case R28_Y_REGISTER_LOW_BYTE: return "R28";
        case R29_Y_REGISTER_HIGH_BYTE: return "R29";
        case R30_Z_REGISTER_LOW_BYTE: return "R30";
        case R31_Z_REGISTER_HIGH_BYTE: return "R31";
        case DDRB: return "DDRB";
        case PORTB: return "PORTB"; 
        case GPIOR0: return "GPIOR0";
        case EECR: return "EECR";
        case EEDR: return "EEDR";
        case EEARL: return "EEARL";
        case EEARH: return "EEARH";
        case GPIOR1: return "GPIOR1";
        case GPIOR2: return "GPIOR2";
        case SPL: return "SPL";
        case SPH: return "SPH";
        case SREG: return "SREG";
        default: return "UNKNOWN_ADDRESS";
    }
}





// UNKNOWN OPCODE
InstructionInfo unknown_disassemble(uint16_t opCode, uint16_t programCounter) {
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
InstructionInfo adc_disassemble(uint16_t opCode, uint16_t programCounter) {
    uint16_t rd_addr = dec_extractBits0000000111110000(opCode);
    uint16_t rr_addr = dec_extractBits0000001000001111(opCode);

    InstructionInfo instruction = {0};

    snprintf(instruction.info, INFO_LENGTH, "adc %s %s", _getName(rd_addr), _getName(rr_addr));
    instruction.length =  1;
    
    return instruction;
}



// ADD - Add without Carry
// 16-bit Opcode: 0000 11rd dddd rrrr
// Adds two registers without the C Flag and places the result in the destination register Rd.
// AVR Instruction Manual page 32
InstructionInfo add_disassemble(uint16_t opCode, uint16_t programCounter) {
    char *instructionName = "add";
    uint16_t rd_addr = dec_extractBits0000000111110000(opCode);
    uint16_t rr_addr = dec_extractBits0000001000001111(opCode);

    InstructionInfo instruction = {0};

    snprintf(instruction.info, INFO_LENGTH, "add %s %s", _getName(rd_addr), _getName(rr_addr));
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
InstructionInfo brne_disassemble(uint16_t opCode, uint16_t programCounter) {
    char *instructionName = "brne";
    int16_t constData = (int16_t)dec_extractBits0000001111111000(opCode);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "brne %04X", programCounter + constData - 127);
    snprintf(instruction.comment, INFO_LENGTH, "; branches if Z (SREG bit 1) is cleared");
    instruction.length =  1;
    
    return instruction;
}




// CALL – Long Call to a Subroutine
// 32-bit Opcode: 1001 010k kkkk 111k : kkkk kkkk kkkk kkkk 
// Calls to a subroutine within the entire Program memory. The return address (to the instruction after the
// CALL) will be stored onto the Stack. (See also RCALL). The Stack Pointer uses a post-decrement
// scheme during CALL.
// AVR Instruction Manual page 63
InstructionInfo call_disassemble(uint16_t opCode, uint16_t programCounter) {
    uint16_t jumpDest_addr = mem_fetchInstruction(programCounter + 1);
    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "call %04X", jumpDest_addr);
    instruction.length = 2;
    
    return instruction;
}




// CBI – Clear Bit in I/O Register
// 16-bit Opcode: 1001 1000 AAAA Abbb
// Clears a specified bit in an I/O register. This instruction operates on the lower 32 I/O registers –
// addresses 0-31.
// AVR Instruction Manual page 65
InstructionInfo cbi_disassemble(uint16_t opCode, uint16_t programCounter) {
    uint16_t ioa_addr = dec_extractBits0000000011111000(opCode) + 0x20;
    uint16_t bitNum = dec_extractBits0000000000000111(opCode);
    uint8_t ioaContent = mem_dataRead8bit(ioa_addr);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "cbi %s(%02X), %1X", _getName(ioa_addr), ioaContent, bitNum);
    instruction.length =  1;
    
    return instruction;
}




// CLI – Clear Global Interrupt Flag
// 16-bit Opcode: 1001 0100 1111 1000 
// Clears the Global Interrupt Flag (I) in SREG (Status Register). The interrupts will be immediately
// disabled. No interrupt will be executed after the CLI instruction, even if it occurs simultaneously with the
// CLI instruction.
// AVR Instruction Manual page 69
InstructionInfo cli_disassemble(uint16_t opCode, uint16_t programCounter) {
    InstructionInfo instruction = {0};

    snprintf(instruction.info, INFO_LENGTH, "cli");
    instruction.length =  1;
    
    return instruction;
}




// CP - Compare
// 16-bit Opcode: 0001 01rd dddd rrrr
// This instruction performs a compare between two registers Rd and Rr. None of the registers are changed.
// All conditional branches can be used after this instruction.
// AVR Instruction Manual page 77
InstructionInfo cp_disassemble(uint16_t opCode, uint16_t programCounter) {
    uint16_t rd_addr = dec_extractBits0000000111110000(opCode);
    uint16_t rr_addr = dec_extractBits0000001000001111(opCode);
    uint8_t rdContent = mem_dataRead8bit(rd_addr);
    uint8_t rrContent = mem_dataRead8bit(rr_addr);
    uint8_t result = rdContent - rrContent;

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "cp \"%s\"(%02X) \"%s\"(%02X)", _getName(rd_addr), rdContent, _getName(rr_addr), rrContent);
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
InstructionInfo dec_disassemble(uint16_t opCode, uint16_t programCounter) {
    char *instructionName = "dec";
    uint16_t rd_addr = dec_extractBits0000000111110000(opCode);
    uint8_t rdContent = mem_dataRead8bit(rd_addr);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "dec %s(%02X)", _getName(rd_addr), rdContent);
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
InstructionInfo eorclr_disassemble(uint16_t opCode, uint16_t programCounter) {
    char *instructionName = "eor";
    uint16_t rd_addr = dec_extractBits0000000111110000(opCode);
    uint16_t rr_addr = dec_extractBits0000001000001111(opCode);
    uint8_t rdContent = mem_dataRead8bit(rd_addr);
    uint8_t rrContent = mem_dataRead8bit(rr_addr);

    InstructionInfo instruction = {0};
 
    if (rd_addr == rr_addr) {
        snprintf(instruction.info, INFO_LENGTH, "clr %s(%02X)", _getName(rd_addr), rdContent);
    } else {
        snprintf(instruction.info, INFO_LENGTH, "eor %s(%02X), %s(%02X)", _getName(rd_addr), rdContent, _getName(rr_addr), rdContent);
    }
    instruction.length =  1;
    
    return instruction;
}



// IN - Load an I/O Location to Register
// 16-bit Opcode: 1011 0AAd dddd AAAA
// Loads data from the I/O Space (Ports, Timers, Configuration Registers, etc.) into register Rd in the
// Register File.
// AVR Instruction Manual page 100
InstructionInfo in_disassemble(uint16_t opCode, uint16_t programCounter) {
    uint16_t ioa_addr = dec_extractBits0000011000001111(opCode) + 0x20;
    uint16_t rr_addr =  dec_extractBits0000000111110000(opCode);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "in %s, %s", _getName(rr_addr), _getName(ioa_addr));
    instruction.length =  1;
    
    return instruction;
}




// JMP – Jump
// 32-bit Opcode: 1001 010k kkkk 110k kkkk kkkk kkkk kkkk
// Jump to an address within the entire 4M (words) Program memory.
// AVR Instruction Manual page 103
InstructionInfo jmp_disassemble(uint16_t opCode, uint16_t programCounter) {
    uint16_t jumpDest_addr = mem_fetchInstruction(programCounter + 1);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "jmp %04X", jumpDest_addr);
    instruction.length =  2;
    
    return instruction;
}




InstructionInfo ldi_disassemble(uint16_t opCode, uint16_t programCounter) {
    uint16_t rd_addr =  dec_extractBits0000000011110000(opCode) + 16;
    uint8_t constData = dec_extractBits0000111100001111(opCode);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "ldi %s, %02X", _getName(rd_addr), constData);
    instruction.length =  1;
    
    return instruction;
}




// LDS – Load Direct from Data Space
// 32-bit Opcode: 1001 000d dddd 0000 : kkkk kkkk kkkk kkkk
// Loads one byte from the data space to a register.
// AVR Instruction Manual page 116
InstructionInfo lds32_disassemble(uint16_t opCode, uint16_t programCounter) {
    uint16_t rd_addr =  dec_extractBits0000000111110000(opCode);
    uint8_t rdContent = mem_dataRead8bit(rd_addr);
    uint8_t constAddress = mem_fetchInstruction(programCounter + 1); 
    uint8_t memContent = mem_dataRead8bit(constAddress);

    InstructionInfo instruction = {0};

    snprintf(instruction.info, INFO_LENGTH, "lds32 %s(%02X), %s(%02X)",  _getName(rd_addr), rdContent, _getName(constAddress), memContent);   
    instruction.length = 2;

    return instruction;
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
InstructionInfo lds16_disassemble(uint16_t opCode, uint16_t programCounter) {
    uint16_t rd_addr =  dec_extractBits0000000011110000(opCode);
    uint8_t rdContent = mem_dataRead8bit(rd_addr);

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

    uint8_t memContent = mem_dataRead8bit(constAddress);

    InstructionInfo instruction = {0};

    snprintf(instruction.info, INFO_LENGTH, "lds16 %s(%02X), %s(%02X)",  _getName(rd_addr), rdContent, _getName(constAddress), memContent);   

    return instruction;
}







InstructionInfo nop_disassemble(uint16_t opCode, uint16_t programCounter) {
    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "nop");

    instruction.length =  1;
    
    return instruction;
}




// ORI – Logical OR with Immediate
// 16-bit Opcode: 0110 KKKK dddd KKKK
// Performs the logical OR between the contents of register Rd and a constant, and places the result in the
// destination register Rd.
// AVR Instruction Manual page 133
InstructionInfo ori_disassemble(uint16_t opCode, uint16_t programCounter) {
    uint16_t rd_addr =  dec_extractBits0000000011110000(opCode) + 16;
    uint8_t constData = dec_extractBits0000111100001111(opCode);
    uint8_t rdContent = mem_dataRead8bit(rd_addr);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "ori %s(%02X), %02X",  _getName(rd_addr), rdContent, constData);

    instruction.length =  1;
    
    return instruction;
}



// OUT – Store Register to I/O Location
// 16-bit Opcode: 1011 1AAr rrrr AAAA
// Stores data from register Rr in the Register File to I/O Space (Ports, Timers, Configuration Registers, etc.).
// When using the I/O specific commands IN and OUT, the I/O addresses 0x00 - 0x3F must be used.
// That means 0x20 must bei added to the provided adresses.
// Atmega328 datasheet page 624
// AVR Instruction Manual page 134
InstructionInfo out_disassemble(uint16_t opCode, uint16_t programCounter) {
    uint16_t ioa_addr = dec_extractBits0000011000001111(opCode) + 0x20;
    uint16_t rr_addr =  dec_extractBits0000000111110000(opCode);
    uint8_t rrContent = mem_dataRead8bit(rr_addr);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "out %s, %s(%02X)",  _getName(ioa_addr), _getName(rr_addr), rrContent);

    instruction.length =  1;
    
    return instruction;
}




// POP - Pop Register from Stack
// 16-bit Opcode: 1001 000d dddd 1111
// This instruction loads register Rd with a byte from the STACK. The Stack Pointer is pre-incremented by 1
// before the POP.
// This instruction is not available in all devices. Refer to the device specific instruction set summary.
// AVR Instruction Manual page 135
InstructionInfo pop_disassemble(uint16_t opCode, uint16_t programCounter) {
    uint16_t rd_addr = dec_extractBits0000000111110000(opCode);
    uint16_t stackPointer = mem_dataRead16bit(STACKPOINTER);
    uint8_t stackContent = mem_dataRead16bit(stackPointer + 1);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "pop %s, stack(%02X)",  _getName(rd_addr), stackContent);

    instruction.length = 1;
    
    return instruction;
}




// PUSH – Push Register on Stack
// 16-bit Opcode: 1001 001d dddd 1111
// This instruction stores the contents of register Rr on the STACK. The Stack Pointer is post-decremented
// by 1 after the PUSH.
// This instruction is not available in all devices. Refer to the device specific instruction set summary.
// AVR Instruction Manual page 136
InstructionInfo push_disassemble(uint16_t opCode, uint16_t programCounter) {
    uint16_t rr_addr = dec_extractBits0000000111110000(opCode);
    uint16_t stackPointer = mem_dataRead8bit(STACKPOINTER);
    uint8_t rrContent = mem_dataRead8bit(rr_addr);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "push %s(%02X)",  _getName(rr_addr), rrContent);

    instruction.length = 1;
    
    return instruction;
}




// RCALL – Relative Call to Subroutine
// 16-bit Opcode: 1101 kkkk kkkk kkkk
// Relative call to an address within PC - 2K + 1 and PC + 2K (words). The return address (the instruction
// after the RCALL) is stored onto the Stack. See also CALL. For AVR microcontrollers with Program
// memory not exceeding 4K words (8KB) this instruction can address the entire memory from every
// address location. The Stack Pointer uses a post-decrement scheme during RCALL.
// AVR Instruction Manual page 137
InstructionInfo rcall_disassemble(uint16_t opCode, uint16_t programCounter) {
    int16_t constAddress = (int16_t)dec_extractBits0000111111111111(opCode);
    uint16_t jumpDest_addr = (programCounter + constAddress - 0xfff) % (DATA_MEMORY_END + 1);    

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "rcall %04X", jumpDest_addr);
}




// RET – Return from Subroutine
// 16-bit Opcode: 1001 0101 0000 1000
// Returns from subroutine. The return address is loaded from the STACK. The Stack Pointer uses a pre-
// increment scheme during RET.
// AVR Instruction Manual page 139
InstructionInfo ret_disassemble(uint16_t opCode, uint16_t programCounter) {
    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "ret");
}








// SBI – Set Bit in I/O Register
// 16-bit Opcode: 1001 1010 AAAA Abbb
// Sets a specified bit in an I/O Register. This instruction operates on the lower 32 I/O Registers –
// addresses 0-31.
// AVR Instruction Manual page 151
InstructionInfo sbi_disassemble(uint16_t opCode, uint16_t programCounter) {

    uint16_t ioa_addr = dec_extractBits0000000011111000(opCode) + 0x20;
    uint16_t bitNum = dec_extractBits0000000000000111(opCode);
    uint8_t ioaContent = mem_dataRead8bit(ioa_addr);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "sbi %s(%02X), %1X", _getName(ioa_addr), ioaContent, bitNum);

    instruction.length =  1;
    
    return instruction;
}




// SBIC – Skip if Bit in I/O Register is Cleared
// This instruction tests a single bit in an I/O Register and skips the next instruction if the bit is cleared. This
// instruction operates on the lower 32 I/O Registers – addresses 0-31.
// AVR Instruction Manual page 152
InstructionInfo sbic_disassemble(uint16_t opCode, uint16_t programCounter) {
    uint16_t ioa_addr = dec_extractBits0000000001111000(opCode) + 0x20;
    uint16_t bitNum = dec_extractBits0000000000000111(opCode);
    uint8_t ioaContent = mem_dataRead8bit(ioa_addr);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "sbic %s", _getName(ioa_addr));

    instruction.length =  1;
    
    return instruction;
}



// SBIS – Skip if Bit in I/O Register is Set
// This instruction tests a single bit in an I/O Register and skips the next instruction if the bit is set. This
// instruction operates on the lower 32 I/O Registers – addresses 0-31.
// AVR Instruction Manual page 153
InstructionInfo sbis_disassemble(uint16_t opCode, uint16_t programCounter) {
    uint16_t ioa_addr = dec_extractBits0000000001111000(opCode) + 0x20;
    uint16_t bitNum = dec_extractBits0000000000000111(opCode);
    uint8_t ioaContent = mem_dataRead8bit(ioa_addr);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "sbis %s", _getName(ioa_addr));

    instruction.length =  1;
    
    return instruction;
}




// SBIW – Subtract Immediate from Word
// 16-bit Opcode: 1001 0111 KKdd KKKK
// Subtracts an immediate value (0-63) from a register pair and places the result in the register pair.
// This instruction operates on the upper four register pairs, and is well suited for operations on the
// Pointer Registers.
// AVR Instruction Manual page 154
InstructionInfo sbiw_disassemble(uint16_t opCode, uint16_t programCounter) {
    uint16_t rd_addr =   dec_extractBits0000000000110000(opCode) * 2 + R24;
    uint16_t constData = dec_extractBits0000000011001111(opCode);
    uint16_t rdContent = mem_dataRead16bit(rd_addr);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "sbiw %s%s(%04X), %04X", _getName(rd_addr), _getName(rd_addr + 1), rdContent, constData);
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
InstructionInfo sts_disassemble(uint16_t opCode, uint16_t programCounter) {
    uint16_t rr_addr = dec_extractBits0000000111110000(opCode);
    uint16_t instructionSecond = mem_fetchInstruction(programCounter + 1);
    uint16_t mem_addr = instructionSecond;
    uint8_t rrContent = mem_dataRead8bit(rr_addr);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "sts %04X \"%s\"(%02X)", mem_addr, _getName(rr_addr), rrContent);
    instruction.length =  2;

    return instruction;
}




// RJMP – Relative Jump
// 16-bit Opcode: 1100 kkkk kkkk kkkk
// Relative jump to an address within PC - 2K +1 and PC + 2K (words). For AVR microcontrollers with
// Program memory not exceeding 4K words (8KB) this instruction can address the entire memory from
// every address location. See also JMP.
// AVR Instruction Manual page 142
InstructionInfo rjmp_disassemble(uint16_t opCode, uint16_t programCounter) {
    uint16_t addressOffset = (uint16_t)dec_extractBits0000011111111111(opCode);
    bool signBit = (bool)dec_extractBits0000100000000000(opCode);
    uint16_t jumpDest_addr;
    if(signBit) {
        jumpDest_addr = (PROGRAM_MEMORY_END + 2 + programCounter + addressOffset - 0x800)  % (PROGRAM_MEMORY_END + 1);
    } else {
        jumpDest_addr = (PROGRAM_MEMORY_END + 2 + programCounter + addressOffset)  % (PROGRAM_MEMORY_END + 1);
    }

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "rjmp %04X", (jumpDest_addr));
    instruction.length =  1;

    return instruction;
}



// SBCI – Subtract Immediate with Carry SBI – Set Bit in I/O Register
// 16-bit Opcode: 0100 KKKK dddd KKKK
// Subtracts a constant from a register and subtracts with the C Flag, and places the result in the destination
// register Rd.
// AVR Instruction Manual page 149
InstructionInfo sbci_disassemble(uint16_t opCode, uint16_t programCounter) {
    uint8_t rd_addr = dec_extractBits0000000111110000(opCode) + 16;
    uint8_t rdContent = mem_dataRead8bit(rd_addr);
    uint8_t constData = dec_extractBits0000111100001111(opCode);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "sbci %s(%02X), %02X", _getName(rd_addr), rdContent, constData);
    instruction.length =  1;

    return instruction;
}




// SUBI – Subtract Immediate
// 16-bit Opcode: 0101 KKKK dddd KKKK
// Subtracts a register and a constant, and places the result in the destination register Rd. This instruction is
// working on Register R16 to R31 and is very well suited for operations on the X, Y, and Z-pointers.
// AVR Instruction Manual page 183
InstructionInfo subi_disassemble(uint16_t opCode, uint16_t programCounter) {
    uint8_t rd_addr = dec_extractBits0000000111110000(opCode) + 16;
    uint8_t rdContent = mem_dataRead8bit(rd_addr);
    uint8_t constData = dec_extractBits0000111100001111(opCode);

    InstructionInfo instruction = {0};
 
    snprintf(instruction.info, INFO_LENGTH, "subi %s(%02X), %02X", _getName(rd_addr), rdContent, constData);
    instruction.length =  1;

    return instruction;
}


