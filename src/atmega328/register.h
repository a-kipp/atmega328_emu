// Read and write access to the registers is provided by the functions in this file. Most of the
// interrupts are catched during a write operation to the particular register. Some registers such
// as the SREG flag register and some interrupt flag registers are relocated and split into single
// bits and therefore must bei accessed only by their read and write functions.


#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "memory/map.h"
#include "declarations.h"
#include "interrupts.h"



static uint8_t _sregRead(uint16_t address) {
    uint8_t returnValue = 0;
    returnValue |= (reg_sregCarry << CARRY_FLAG);
    returnValue |= (reg_sregZero << ZERO_FLAG);
    returnValue |= (reg_sregNegative << NEGATIVE_FLAG);
    returnValue |= (reg_sregTwoComplOverflow << TWOS_COMPLEMENT_OVERFLOW_FLAG);
    returnValue |= (reg_sregSignBit << SIGN_BIT);
    returnValue |= (reg_sregHalfCarry << HALF_CARRY_FLAG);
    returnValue |= (reg_sregBitCopyStorage << BIT_COPY_STORAGE);
    returnValue |= (reg_sregGlobalInterruptEnable << GLOBAL_INTERRUPT_ENABLE);
    return returnValue;
}


static void _sregWrite(uint16_t address, uint8_t value) {
    reg_sregCarry = value & (1 << CARRY_FLAG);
    reg_sregZero  = value & (1 << ZERO_FLAG);
    reg_sregNegative = value & (1 << NEGATIVE_FLAG);
    reg_sregTwoComplOverflow = value & (1 << TWOS_COMPLEMENT_OVERFLOW_FLAG);
    reg_sregSignBit = value & (1 << SIGN_BIT);
    reg_sregHalfCarry = value & (1 << HALF_CARRY_FLAG);
    reg_sregBitCopyStorage = value & (1 << BIT_COPY_STORAGE);
    reg_sregGlobalInterruptEnable = value & (1 << GLOBAL_INTERRUPT_ENABLE);
}


static uint8_t _pcifrRead0x3B(uint16_t address) {
    uint8_t returnValue = 0;
    returnValue |= (reg_pinChangeInterruptFlag2 << PCIF2);
    returnValue |= (reg_pinChangeInterruptFlag1 << PCIF1);
    returnValue |= (reg_pinChangeInterruptFlag0 << PCIF0);
    return returnValue;
}


static void _pcifrWrite0x3B(uint16_t address, uint8_t value) {
    reg_pinChangeInterruptFlag2 = value & (1 << PCIF2);
    reg_pinChangeInterruptFlag1 = value & (1 << PCIF1);
    reg_pinChangeInterruptFlag0 = value & (1 << PCIF0);
}


static uint8_t _eifrRead0x3C(uint16_t address) {
    uint8_t returnValue = 0;
    returnValue |= (reg_externalInterruptFlag1 << INTF1);
    returnValue |= (reg_externalInterruptFlag0 << INTF0);
    return returnValue;
}


static void _eifrWrite0x3C(uint16_t address, uint8_t value) {
    reg_externalInterruptFlag1 = value & (1 << INTF1);
    reg_externalInterruptFlag0 = value & (1 << INTF0);
}


static uint8_t _eimskRead0x3D(uint16_t address) {
    uint8_t returnValue = 0;
    returnValue |= (reg_externalInterruptMask1 << INT1);
    returnValue |= (reg_externalInterruptMask0 << INT0);
    return returnValue;
}


static void _eimskWrite0x3D(uint16_t address, uint8_t value) {
    reg_externalInterruptMask1 = value & (1 << INT1);
    reg_externalInterruptMask0 = value & (1 << INT0);
}


static uint8_t _pcicrRead0x68(uint16_t address) {
    uint8_t returnValue = 0;
    returnValue |= (reg_pinChangeInterruptControl2 << PCIE2);
    returnValue |= (reg_pinChangeInterruptControl1 << PCIE1);
    returnValue |= (reg_pinChangeInterruptControl0 << PCIE0);
    return returnValue;
}


static void _pcicrWrite0x68(uint16_t address, uint8_t value) {
    reg_pinChangeInterruptControl2 = value & (1 << PCIE2);
    reg_pinChangeInterruptControl1 = value & (1 << PCIE1);
    reg_pinChangeInterruptControl0 = value & (1 << PCIE0);
}

static uint8_t _eicraRead0x69(uint16_t address) {
    uint8_t returnValue = 0;
    returnValue |= (reg_externalInterruptControl3 << ISC11);
    returnValue |= (reg_externalInterruptControl2 << ISC10);
    returnValue |= (reg_externalInterruptControl1 << ISC01);
    returnValue |= (reg_externalInterruptControl0 << ISC00);
    return returnValue;
}


static void _eicraWrite0x69(uint16_t address, uint8_t value) {
    reg_externalInterruptControl3 = value & (1 << ISC11);
    reg_externalInterruptControl2 = value & (1 << ISC10);
    reg_externalInterruptControl1 = value & (1 << ISC01);
    reg_externalInterruptControl0 = value & (1 << ISC00);
}



static void _consolePrint(uint16_t address, uint8_t value) {
    switch (address) {
        case CONSOLE_PRINT_BIN: out_serialOutBin(value); break;
        case CONSOLE_PRINT_CHAR: out_serialOutChar(value); break;
        case CONSOLE_PRINT_DEC: out_serialOutDec(value); break;
        case CONSOLE_PRINT_HEX: out_serialOutHex(value); break;
        case UDR0: out_serialOutChar(value); break;
        default: fprintf(stderr, "this is not a register to print to\n"); break;
    }
}


static void _catchBadAddress(uint16_t address, uint8_t value) {
    if (address == BAD_ADDRESS) {
        fprintf(stderr, "writing to the bad address\n");
    }
}


static void _pinbWrite0x23(uint16_t address, uint8_t newValue) {
    uint8_t pindContent = mem_dataMemory[PINB];
    // match PCINT0 Interrupt
    if (pindContent ^ newValue) int_catchPcint0Interrupt(pindContent ^ newValue);
    mem_dataMemory[PINB] = newValue;
}


static uint8_t _pinbRead0x23(uint16_t address) {
    uint8_t pinbContent = mem_dataMemory[PINB];
    uint8_t portbContent = mem_dataMemory[PORTB];
    return portbContent | pinbContent;
}


static void _portbWrite0x25(uint16_t address, uint8_t value) {
    if (address = PORTB) {
        uint8_t oldHighPins = mem_dataMemory[PORTB] & mem_dataMemory[DDRB];
        mem_dataMemory[PORTB] = value;
        uint8_t newHighPins = value & mem_dataMemory[DDRB];
        uint8_t changedPins = oldHighPins ^ newHighPins;
        if (changedPins & (1 << 7)) per_digitalPinOut(10, newHighPins & (1 << 7));
        if (changedPins & (1 << 6)) per_digitalPinOut(9, newHighPins & (1 << 6));
        if (changedPins & (1 << 5)) per_digitalPinOut(19, newHighPins & (1 << 5));
        if (changedPins & (1 << 3)) per_digitalPinOut(18, newHighPins & (1 << 3));
        if (changedPins & (1 << 2)) per_digitalPinOut(17, newHighPins & (1 << 2));
        if (changedPins & (1 << 4)) per_digitalPinOut(16, newHighPins & (1 << 4));
        if (changedPins & (1 << 1)) per_digitalPinOut(15, newHighPins & (1 << 1));
        if (changedPins & (1 << 0)) per_digitalPinOut(14, newHighPins & (1 << 0));
    } else {
        fprintf(stderr, "something wrong with the pins\n");
    }
}


static void _pincWrite0x26(uint16_t address, uint8_t newValue) {
    uint8_t pindContent = mem_dataMemory[PINC];
    // match PCINT1 Interrupt
    if (pindContent ^ newValue) int_catchPcint0Interrupt(pindContent ^ newValue);
    mem_dataMemory[PINC] = newValue;
}


static uint8_t _pincRead0x26(uint16_t address) {
    uint8_t pincContent = mem_dataMemory[PINC];
    uint8_t portcContent = mem_dataMemory[PORTC];
    return portcContent | pincContent;
}


static void _portcWrite0x28(uint16_t address, uint8_t value) {
    if (address = PORTC) {
        uint8_t oldHighPins = mem_dataMemory[PORTC] & mem_dataMemory[DDRC];
        mem_dataMemory[PORTC] = value;
        uint8_t newHighPins = value & mem_dataMemory[DDRC];
        uint8_t changedPins = oldHighPins ^ newHighPins;
        if (changedPins & (1 << 6)) per_digitalPinOut(1, newHighPins & (1 << 6));
        if (changedPins & (1 << 5)) per_digitalPinOut(28, newHighPins & (1 << 5));
        if (changedPins & (1 << 3)) per_digitalPinOut(27, newHighPins & (1 << 3));
        if (changedPins & (1 << 2)) per_digitalPinOut(26, newHighPins & (1 << 2));
        if (changedPins & (1 << 4)) per_digitalPinOut(25, newHighPins & (1 << 4));
        if (changedPins & (1 << 1)) per_digitalPinOut(24, newHighPins & (1 << 1));
        if (changedPins & (1 << 0)) per_digitalPinOut(22, newHighPins & (1 << 0));
    } else {
        fprintf(stderr, "something wrong with the pins\n");
    }
}


static void _pindWrite0x29(uint16_t address, uint8_t newValue) {
    uint8_t pindContent = mem_dataMemory[PIND];
    uint8_t sregContent = mem_dataMemory[SREG];
    bool oldPd2 = (bool)(pindContent & (1 << PORTD2_PIN_4));
    bool newPd2 = (bool)(newValue & (1 << PORTD2_PIN_4));
    bool oldPd3 = (bool)(pindContent & (1 << PIND3_PIN_5));
    bool newPd3 = (bool)(newValue & (1 << PIND3_PIN_5));

    // match External Interrupt Request 0
    if (!oldPd2 && newPd2) int_catchInt0Interrupt(RISING);
    if (oldPd2 && !newPd2) int_catchInt0Interrupt(FALLING);
    if (oldPd2 ^ newPd2) int_catchInt0Interrupt(CHANGING);

    // match External Interrupt Request 1
    if (!oldPd3 && newPd3) int_catchInt1Interrupt(RISING);
    if (oldPd3 && !newPd3) int_catchInt1Interrupt(FALLING);
    if (oldPd3 ^ newPd3) int_catchInt1Interrupt(CHANGING);

    // match PCINT2 Interrupt
    if (pindContent ^ newValue) int_catchPcint2Interrupt(pindContent ^ newValue);

    mem_dataMemory[PIND] = newValue;
}


static uint8_t _pinbRead0x29(uint16_t address) {
    uint8_t pindContent = mem_dataMemory[PIND];
    uint8_t portdContent = mem_dataMemory[PORTD];
    return portdContent | pindContent;
}


static void _portdWrite0x2B(uint16_t address, uint8_t value) {
    if (address = PORTD) {
        uint8_t oldHighPins = mem_dataMemory[PORTD] & mem_dataMemory[DDRD];
        mem_dataMemory[PORTD] = value;
        uint8_t newHighPins = value & mem_dataMemory[DDRD];
        uint8_t changedPins = oldHighPins ^ newHighPins;
        if (changedPins & (1 << 7)) per_digitalPinOut(13, newHighPins & (1 << 7));
        if (changedPins & (1 << 6)) per_digitalPinOut(12, newHighPins & (1 << 6));
        if (changedPins & (1 << 5)) per_digitalPinOut(11, newHighPins & (1 << 5));
        if (changedPins & (1 << 3)) per_digitalPinOut(6, newHighPins & (1 << 3));
        if (changedPins & (1 << 2)) per_digitalPinOut(5, newHighPins & (1 << 2));
        if (changedPins & (1 << 4)) per_digitalPinOut(4, newHighPins & (1 << 4));
        if (changedPins & (1 << 1)) per_digitalPinOut(3, newHighPins & (1 << 1));
        if (changedPins & (1 << 0)) per_digitalPinOut(2, newHighPins & (1 << 0));
    } else {
        fprintf(stderr, "something wrong with the pins\n");
    }
}


static void _writeToPinInput(uint16_t address, uint8_t value) {
    fprintf(stderr, "tried to write to pin input register, nothing happened\n");
}


static uint8_t _unconditionalRead(uint16_t address) {
    return mem_dataMemory[address];
}


static void _unconditionalWrite(uint16_t address, uint8_t value) {
    mem_dataMemory[address] = value;
}


static uint8_t(*_registerReadFunctions[])(uint16_t) = {
    _unconditionalRead, // 0x00
    _unconditionalRead, // 0x01
    _unconditionalRead, // 0x02
    _unconditionalRead, // 0x03
    _unconditionalRead, // 0x04
    _unconditionalRead, // 0x05
    _unconditionalRead, // 0x06
    _unconditionalRead, // 0x07
    _unconditionalRead, // 0x08
    _unconditionalRead, // 0x09
    _unconditionalRead, // 0x0A
    _unconditionalRead, // 0x0B
    _unconditionalRead, // 0x0C
    _unconditionalRead, // 0x0D
    _unconditionalRead, // 0x0E
    _unconditionalRead, // 0x0F
    _unconditionalRead, // 0x10
    _unconditionalRead, // 0x11
    _unconditionalRead, // 0x12
    _unconditionalRead, // 0x13
    _unconditionalRead, // 0x14
    _unconditionalRead, // 0x15
    _unconditionalRead, // 0x16
    _unconditionalRead, // 0x17
    _unconditionalRead, // 0x18
    _unconditionalRead, // 0x19
    _unconditionalRead, // 0x1A
    _unconditionalRead, // 0x1B
    _unconditionalRead, // 0x1C
    _unconditionalRead, // 0x1D
    _unconditionalRead, // 0x1E
    _unconditionalRead, // 0x1F
    _unconditionalRead, // 0x20
    _unconditionalRead, // 0x21
    _unconditionalRead, // 0x22
    _pinbRead0x23, // 0x23
    _unconditionalRead, // 0x24
    _unconditionalRead, // 0x25
    _pincRead0x26, // 0x26
    _unconditionalRead, // 0x27
    _unconditionalRead, // 0x28
    _pinbRead0x29, // 0x29
    _unconditionalRead, // 0x2A
    _unconditionalRead, // 0x2B
    _unconditionalRead, // 0x2C
    _unconditionalRead, // 0x2D
    _unconditionalRead, // 0x2E
    _unconditionalRead, // 0x2F
    _unconditionalRead, // 0x30
    _unconditionalRead, // 0x31
    _unconditionalRead, // 0x32
    _unconditionalRead, // 0x33
    _unconditionalRead, // 0x34
    _unconditionalRead, // 0x35
    _unconditionalRead, // 0x36
    _unconditionalRead, // 0x37
    _unconditionalRead, // 0x38
    _unconditionalRead, // 0x39
    _unconditionalRead, // 0x3A
    _pcifrRead0x3B, // 0x3B
    _eifrRead0x3C, // 0x3C
    _eimskRead0x3D, // 0x3D
    _unconditionalRead, // 0x3E
    _unconditionalRead, // 0x3F
    _unconditionalRead, // 0x40
    _unconditionalRead, // 0x41
    _unconditionalRead, // 0x42
    _unconditionalRead, // 0x43
    _unconditionalRead, // 0x44
    _unconditionalRead, // 0x45
    _unconditionalRead, // 0x46
    _unconditionalRead, // 0x47
    _unconditionalRead, // 0x48
    _unconditionalRead, // 0x49
    _unconditionalRead, // 0x4A
    _unconditionalRead, // 0x4B
    _unconditionalRead, // 0x4C
    _unconditionalRead, // 0x4D
    _unconditionalRead, // 0x4E
    _unconditionalRead, // 0x4F
    _unconditionalRead, // 0x50
    _unconditionalRead, // 0x51
    _unconditionalRead, // 0x52
    _unconditionalRead, // 0x53
    _unconditionalRead, // 0x54
    _unconditionalRead, // 0x55
    _unconditionalRead, // 0x56
    _unconditionalRead, // 0x57
    _unconditionalRead, // 0x58
    _unconditionalRead, // 0x59
    _unconditionalRead, // 0x5A
    _unconditionalRead, // 0x5B
    _unconditionalRead, // 0x5C
    _unconditionalRead, // 0x5D
    _unconditionalRead, // 0x5E
    _sregRead, // 0x5F
    _unconditionalRead, // 0x60
    _unconditionalRead, // 0x61
    _unconditionalRead, // 0x62
    _unconditionalRead, // 0x63
    _unconditionalRead, // 0x64
    _unconditionalRead, // 0x65
    _unconditionalRead, // 0x66
    _unconditionalRead, // 0x67
    _pcicrRead0x68, // 0x68
    _eicraRead0x69, // 0x69
    _unconditionalRead, // 0x6A
    _unconditionalRead, // 0x6B
    _unconditionalRead, // 0x6C
    _unconditionalRead, // 0x6D
    _unconditionalRead, // 0x6E
    _unconditionalRead, // 0x6F
    _unconditionalRead, // 0x70
    _unconditionalRead, // 0x71
    _unconditionalRead, // 0x72
    _unconditionalRead, // 0x73
    _unconditionalRead, // 0x74
    _unconditionalRead, // 0x75
    _unconditionalRead, // 0x76
    _unconditionalRead, // 0x77
    _unconditionalRead, // 0x78
    _unconditionalRead, // 0x79
    _unconditionalRead, // 0x7A
    _unconditionalRead, // 0x7B
    _unconditionalRead, // 0x7C
    _unconditionalRead, // 0x7D
    _unconditionalRead, // 0x7E
    _unconditionalRead, // 0x7F
    _unconditionalRead, // 0x80
    _unconditionalRead, // 0x81
    _unconditionalRead, // 0x82
    _unconditionalRead, // 0x83
    _unconditionalRead, // 0x84
    _unconditionalRead, // 0x85
    _unconditionalRead, // 0x86
    _unconditionalRead, // 0x87
    _unconditionalRead, // 0x88
    _unconditionalRead, // 0x89
    _unconditionalRead, // 0x8A
    _unconditionalRead, // 0x8B
    _unconditionalRead, // 0x8C
    _unconditionalRead, // 0x8D
    _unconditionalRead, // 0x8E
    _unconditionalRead, // 0x8F
    _unconditionalRead, // 0x90
    _unconditionalRead, // 0x91
    _unconditionalRead, // 0x92
    _unconditionalRead, // 0x93
    _unconditionalRead, // 0x94
    _unconditionalRead, // 0x95
    _unconditionalRead, // 0x96
    _unconditionalRead, // 0x97
    _unconditionalRead, // 0x98
    _unconditionalRead, // 0x99
    _unconditionalRead, // 0x9A
    _unconditionalRead, // 0x9B
    _unconditionalRead, // 0x9C
    _unconditionalRead, // 0x9D
    _unconditionalRead, // 0x9E
    _unconditionalRead, // 0x9F
    _unconditionalRead, // 0xA0
    _unconditionalRead, // 0xA1
    _unconditionalRead, // 0xA2
    _unconditionalRead, // 0xA3
    _unconditionalRead, // 0xA4
    _unconditionalRead, // 0xA5
    _unconditionalRead, // 0xA6
    _unconditionalRead, // 0xA7
    _unconditionalRead, // 0xA8
    _unconditionalRead, // 0xA9
    _unconditionalRead, // 0xAA
    _unconditionalRead, // 0xAB
    _unconditionalRead, // 0xAC
    _unconditionalRead, // 0xAD
    _unconditionalRead, // 0xAE
    _unconditionalRead, // 0xAF
    _unconditionalRead, // 0xB0
    _unconditionalRead, // 0xB1
    _unconditionalRead, // 0xB2
    _unconditionalRead, // 0xB3
    _unconditionalRead, // 0xB4
    _unconditionalRead, // 0xB5
    _unconditionalRead, // 0xB6
    _unconditionalRead, // 0xB7
    _unconditionalRead, // 0xB8
    _unconditionalRead, // 0xB9
    _unconditionalRead, // 0xBA
    _unconditionalRead, // 0xBB
    _unconditionalRead, // 0xBC
    _unconditionalRead, // 0xBD
    _unconditionalRead, // 0xBE
    _unconditionalRead, // 0xBF
    _unconditionalRead, // 0xC0
    _unconditionalRead, // 0xC1
    _unconditionalRead, // 0xC2
    _unconditionalRead, // 0xC3
    _unconditionalRead, // 0xC4
    _unconditionalRead, // 0xC5
    _unconditionalRead, // 0xC6
    _unconditionalRead, // 0xC7
    _unconditionalRead, // 0xC8
    _unconditionalRead, // 0xC9
    _unconditionalRead, // 0xCA
    _unconditionalRead, // 0xCB
    _unconditionalRead, // 0xCC
    _unconditionalRead, // 0xCD
    _unconditionalRead, // 0xCE
    _unconditionalRead, // 0xCF
    _unconditionalRead, // 0xD0
    _unconditionalRead, // 0xD1
    _unconditionalRead, // 0xD2
    _unconditionalRead, // 0xD3
    _unconditionalRead, // 0xD4
    _unconditionalRead, // 0xD5
    _unconditionalRead, // 0xD6
    _unconditionalRead, // 0xD7
    _unconditionalRead, // 0xD8
    _unconditionalRead, // 0xD9
    _unconditionalRead, // 0xDA
    _unconditionalRead, // 0xDB
    _unconditionalRead, // 0xDC
    _unconditionalRead, // 0xDD
    _unconditionalRead, // 0xDE
    _unconditionalRead, // 0xDF
    _unconditionalRead, // 0xE0
    _unconditionalRead, // 0xE1
    _unconditionalRead, // 0xE2
    _unconditionalRead, // 0xE3
    _unconditionalRead, // 0xE4
    _unconditionalRead, // 0xE5
    _unconditionalRead, // 0xE6
    _unconditionalRead, // 0xE7
    _unconditionalRead, // 0xE8
    _unconditionalRead, // 0xE9
    _unconditionalRead, // 0xEA
    _unconditionalRead, // 0xEB
    _unconditionalRead, // 0xEC
    _unconditionalRead, // 0xED
    _unconditionalRead, // 0xEE
    _unconditionalRead, // 0xEF
    _unconditionalRead, // 0xF0
    _unconditionalRead, // 0xF1
    _unconditionalRead, // 0xF2
    _unconditionalRead, // 0xF3
    _unconditionalRead, // 0xF4
    _unconditionalRead, // 0xF5
    _unconditionalRead, // 0xF6
    _unconditionalRead, // 0xF7
    _unconditionalRead, // 0xF8
    _unconditionalRead, // 0xF9
    _unconditionalRead, // 0xFA
    _unconditionalRead, // 0xFB
    _unconditionalRead, // 0xFC
    _unconditionalRead, // 0xFD
    _unconditionalRead, // 0xFE
    _unconditionalRead, // 0xFF
};


static void(*_registerWriteFunctions[])(uint16_t, uint8_t) = {
    _unconditionalWrite, // 0x00
    _unconditionalWrite, // 0x01
    _unconditionalWrite, // 0x02
    _unconditionalWrite, // 0x03
    _unconditionalWrite, // 0x04
    _unconditionalWrite, // 0x05
    _unconditionalWrite, // 0x06
    _unconditionalWrite, // 0x07
    _unconditionalWrite, // 0x08
    _unconditionalWrite, // 0x09
    _unconditionalWrite, // 0x0A
    _unconditionalWrite, // 0x0B
    _unconditionalWrite, // 0x0C
    _unconditionalWrite, // 0x0D
    _unconditionalWrite, // 0x0E
    _unconditionalWrite, // 0x0F
    _unconditionalWrite, // 0x10
    _unconditionalWrite, // 0x11
    _unconditionalWrite, // 0x12
    _unconditionalWrite, // 0x13
    _unconditionalWrite, // 0x14
    _unconditionalWrite, // 0x15
    _unconditionalWrite, // 0x16
    _unconditionalWrite, // 0x17
    _unconditionalWrite, // 0x18
    _unconditionalWrite, // 0x19
    _unconditionalWrite, // 0x1A
    _unconditionalWrite, // 0x1B
    _unconditionalWrite, // 0x1C
    _unconditionalWrite, // 0x1D
    _unconditionalWrite, // 0x1E
    _unconditionalWrite, // 0x1F
    _unconditionalWrite, // 0x20
    _unconditionalWrite, // 0x21
    _unconditionalWrite, // 0x22
    _pinbWrite0x23, // 0x23
    _unconditionalWrite, // 0x24
    _portbWrite0x25, // 0x25
    _pincWrite0x26, // 0x26
    _unconditionalWrite, // 0x27
    _portcWrite0x28, // 0x28
    _pindWrite0x29, // 0x29
    _unconditionalWrite, // 0x2A
    _portdWrite0x2B, // 0x2B
    _unconditionalWrite, // 0x2C
    _unconditionalWrite, // 0x2D
    _unconditionalWrite, // 0x2E
    _unconditionalWrite, // 0x2F
    _unconditionalWrite, // 0x30
    _unconditionalWrite, // 0x31
    _unconditionalWrite, // 0x32
    _unconditionalWrite, // 0x33
    _unconditionalWrite, // 0x34
    _unconditionalWrite, // 0x35
    _unconditionalWrite, // 0x36
    _unconditionalWrite, // 0x37
    _unconditionalWrite, // 0x38
    _unconditionalWrite, // 0x39
    _unconditionalWrite, // 0x3A
    _pcifrWrite0x3B, // 0x3B
    _eifrWrite0x3C, // 0x3C
    _eimskWrite0x3D, // 0x3D
    _unconditionalWrite, // 0x3E
    _unconditionalWrite, // 0x3F
    _unconditionalWrite, // 0x40
    _unconditionalWrite, // 0x41
    _unconditionalWrite, // 0x42
    _unconditionalWrite, // 0x43
    _unconditionalWrite, // 0x44
    _unconditionalWrite, // 0x45
    _unconditionalWrite, // 0x46
    _unconditionalWrite, // 0x47
    _unconditionalWrite, // 0x48
    _unconditionalWrite, // 0x49
    _unconditionalWrite, // 0x4A
    _unconditionalWrite, // 0x4B
    _unconditionalWrite, // 0x4C
    _unconditionalWrite, // 0x4D
    _unconditionalWrite, // 0x4E
    _unconditionalWrite, // 0x4F
    _unconditionalWrite, // 0x50
    _unconditionalWrite, // 0x51
    _unconditionalWrite, // 0x52
    _unconditionalWrite, // 0x53
    _unconditionalWrite, // 0x54
    _unconditionalWrite, // 0x55
    _unconditionalWrite, // 0x56
    _unconditionalWrite, // 0x57
    _unconditionalWrite, // 0x58
    _unconditionalWrite, // 0x59
    _unconditionalWrite, // 0x5A
    _unconditionalWrite, // 0x5B
    _unconditionalWrite, // 0x5C
    _unconditionalWrite, // 0x5D
    _unconditionalWrite, // 0x5E
    _sregWrite, // 0x5F
    _unconditionalWrite, // 0x60
    _unconditionalWrite, // 0x61
    _unconditionalWrite, // 0x62
    _unconditionalWrite, // 0x63
    _unconditionalWrite, // 0x64
    _unconditionalWrite, // 0x65
    _unconditionalWrite, // 0x66
    _unconditionalWrite, // 0x67
    _pcicrWrite0x68, // 0x68
    _eicraWrite0x69, // 0x69
    _unconditionalWrite, // 0x6A
    _unconditionalWrite, // 0x6B
    _unconditionalWrite, // 0x6C
    _unconditionalWrite, // 0x6D
    _unconditionalWrite, // 0x6E
    _unconditionalWrite, // 0x6F
    _unconditionalWrite, // 0x70
    _unconditionalWrite, // 0x71
    _unconditionalWrite, // 0x72
    _unconditionalWrite, // 0x73
    _unconditionalWrite, // 0x74
    _unconditionalWrite, // 0x75
    _unconditionalWrite, // 0x76
    _unconditionalWrite, // 0x77
    _unconditionalWrite, // 0x78
    _unconditionalWrite, // 0x79
    _unconditionalWrite, // 0x7A
    _unconditionalWrite, // 0x7B
    _unconditionalWrite, // 0x7C
    _unconditionalWrite, // 0x7D
    _unconditionalWrite, // 0x7E
    _unconditionalWrite, // 0x7F
    _unconditionalWrite, // 0x80
    _unconditionalWrite, // 0x81
    _unconditionalWrite, // 0x82
    _unconditionalWrite, // 0x83
    _unconditionalWrite, // 0x84
    _unconditionalWrite, // 0x85
    _unconditionalWrite, // 0x86
    _unconditionalWrite, // 0x87
    _unconditionalWrite, // 0x88
    _unconditionalWrite, // 0x89
    _unconditionalWrite, // 0x8A
    _unconditionalWrite, // 0x8B
    _unconditionalWrite, // 0x8C
    _unconditionalWrite, // 0x8D
    _unconditionalWrite, // 0x8E
    _unconditionalWrite, // 0x8F
    _unconditionalWrite, // 0x90
    _unconditionalWrite, // 0x91
    _unconditionalWrite, // 0x92
    _unconditionalWrite, // 0x93
    _unconditionalWrite, // 0x94
    _unconditionalWrite, // 0x95
    _unconditionalWrite, // 0x96
    _unconditionalWrite, // 0x97
    _unconditionalWrite, // 0x98
    _unconditionalWrite, // 0x99
    _unconditionalWrite, // 0x9A
    _unconditionalWrite, // 0x9B
    _unconditionalWrite, // 0x9C
    _unconditionalWrite, // 0x9D
    _unconditionalWrite, // 0x9E
    _unconditionalWrite, // 0x9F
    _unconditionalWrite, // 0xA0
    _unconditionalWrite, // 0xA1
    _unconditionalWrite, // 0xA2
    _unconditionalWrite, // 0xA3
    _unconditionalWrite, // 0xA4
    _unconditionalWrite, // 0xA5
    _unconditionalWrite, // 0xA6
    _unconditionalWrite, // 0xA7
    _unconditionalWrite, // 0xA8
    _unconditionalWrite, // 0xA9
    _unconditionalWrite, // 0xAA
    _unconditionalWrite, // 0xAB
    _unconditionalWrite, // 0xAC
    _unconditionalWrite, // 0xAD
    _unconditionalWrite, // 0xAE
    _unconditionalWrite, // 0xAF
    _unconditionalWrite, // 0xB0
    _unconditionalWrite, // 0xB1
    _unconditionalWrite, // 0xB2
    _unconditionalWrite, // 0xB3
    _unconditionalWrite, // 0xB4
    _unconditionalWrite, // 0xB5
    _unconditionalWrite, // 0xB6
    _unconditionalWrite, // 0xB7
    _unconditionalWrite, // 0xB8
    _unconditionalWrite, // 0xB9
    _unconditionalWrite, // 0xBA
    _unconditionalWrite, // 0xBB
    _unconditionalWrite, // 0xBC
    _unconditionalWrite, // 0xBD
    _unconditionalWrite, // 0xBE
    _unconditionalWrite, // 0xBF
    _unconditionalWrite, // 0xC0
    _unconditionalWrite, // 0xC1
    _unconditionalWrite, // 0xC2
    _unconditionalWrite, // 0xC3
    _unconditionalWrite, // 0xC4
    _unconditionalWrite, // 0xC5
    _consolePrint, // 0xC6
    _unconditionalWrite, // 0xC7
    _unconditionalWrite, // 0xC8
    _unconditionalWrite, // 0xC9
    _unconditionalWrite, // 0xCA
    _unconditionalWrite, // 0xCB
    _unconditionalWrite, // 0xCC
    _unconditionalWrite, // 0xCD
    _unconditionalWrite, // 0xCE
    _unconditionalWrite, // 0xCF
    _unconditionalWrite, // 0xD0
    _unconditionalWrite, // 0xD1
    _unconditionalWrite, // 0xD2
    _unconditionalWrite, // 0xD3
    _unconditionalWrite, // 0xD4
    _unconditionalWrite, // 0xD5
    _unconditionalWrite, // 0xD6
    _unconditionalWrite, // 0xD7
    _unconditionalWrite, // 0xD8
    _unconditionalWrite, // 0xD9
    _unconditionalWrite, // 0xDA
    _unconditionalWrite, // 0xDB
    _unconditionalWrite, // 0xDC
    _unconditionalWrite, // 0xDD
    _unconditionalWrite, // 0xDE
    _unconditionalWrite, // 0xDF
    _unconditionalWrite, // 0xE0
    _unconditionalWrite, // 0xE1
    _unconditionalWrite, // 0xE2
    _unconditionalWrite, // 0xE3
    _unconditionalWrite, // 0xE4
    _unconditionalWrite, // 0xE5
    _unconditionalWrite, // 0xE6
    _unconditionalWrite, // 0xE7
    _unconditionalWrite, // 0xE8
    _unconditionalWrite, // 0xE9
    _unconditionalWrite, // 0xEA
    _unconditionalWrite, // 0xEB
    _unconditionalWrite, // 0xEC
    _unconditionalWrite, // 0xED
    _unconditionalWrite, // 0xEE
    _unconditionalWrite, // 0xEF
    _unconditionalWrite, // 0xF0
    _unconditionalWrite, // 0xF1
    _unconditionalWrite, // 0xF2
    _unconditionalWrite, // 0xF3
    _unconditionalWrite, // 0xF4
    _unconditionalWrite, // 0xF5
    _unconditionalWrite, // 0xF6
    _unconditionalWrite, // 0xF7
    _unconditionalWrite, // 0xF8
    _catchBadAddress, // 0xF9
    _consolePrint, // 0xFA
    _consolePrint, // 0xFB
    _consolePrint, // 0xFC
    _consolePrint, // 0xFD
    _consolePrint, // 0xFE
    _unconditionalWrite, // 0xFF
};





// Public
// ____________________________________________________________________________________________________________________



uint8_t reg_read8BitFromRegister(uint16_t address) {
    if (address <= 0xFF) {
        return _registerReadFunctions[address](address);
    } else {
        fprintf(stderr, "the called address is not a register\n");
    }
}


uint8_t reg_write8BitToRegister(uint16_t address, uint8_t value) {
    if(address <= 0xFF) {
        _registerWriteFunctions[address](address, value);
    } else {
        fprintf(stderr, "the called address is not a register\n");
    }
}









