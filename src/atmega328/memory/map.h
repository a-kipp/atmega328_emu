// Due to the harvard architecture of ATmega328 there are distinct memory spaces for
// instructions and data. The program memory is read only when the atmega is running,
// The data memory holds all the state information of the atmega, it serves not only
// as RAM but also holds all the register files in it's address space.
// In addition to that there is an eeprom wich can be thought of as the atmega's hdd.
// All three memorys have their own address space.

#pragma once

// Program Memory Map
#define PROAGRAM_MEMORY_START 0x000
    #define BOOT_SECTION_START
    #define BOOT_SECTION_END 0x7ff
#define PROGRAM_MEMORY_END 0x3fff

// Data Memory Map, also contains CPU registers
#define DATA_MEMORY_START 0x0000
    #define GENERAL_PURPOSE_REGISTERS_START 0x0000
        #define R0 0x0000
        #define R1 0x0001
        #define R2 0x0002
        #define R3 0x0003
        #define R4 0x0004
        #define R5 0x0005
        #define R6 0x0006
        #define R7 0x0007
        #define R8 0x0008
        #define R9 0x0009
        #define R10 0x000A
        #define R11 0x000B
        #define R12 0x000C
        #define R13 0x000D
        #define R14 0x000E
        #define R15 0x000F
        #define R16 0x0010
        #define R17 0x0011
        #define R18 0x0012
        #define R19 0x0013
        #define R20 0x0014
        #define R21 0x0015
        #define R22 0x0016
        #define R23 0x0017
        #define R24 0x0018
        #define R25 0x0019
        #define R26_X_REGISTER_LOW_BYTE 0x001A
        #define R27_X_REGISTER_HIGH_BYTE 0x001B
        #define R28_Y_REGISTER_LOW_BYTE 0x001C
        #define R29_Y_REGISTER_HIGH_BYTE 0x001D
        #define R30_Z_REGISTER_LOW_BYTE 0x001E
        #define R31_Z_REGISTER_HIGH_BYTE 0x001F
    #define GENERAL_PURPOSE_REGISTERS_END 0x001F
    #define IO_REGISTERS_START 0x0020
        #define PINB 0x0023 // The Port B Input Pins Address
        #define DDRB 0x0024 // The Port B Data Direction Register
        #define PORTB 0x0025 // The Port B Data Register
            #define PORTB7_PIN_10 7
            #define PORTB6_PIN_9 6
            #define PORTB5_PIN_19 5
            #define PORTB4_PIN_18 4
            #define PORTB3_PIN_17 3
            #define PORTB2_PIN_16 2 
            #define PORTB1_PIN_15 1
            #define PORTB0_PIN_14 0
        #define PINC 0x0026 // The Port C Input Pins Address
        #define DDRC 0x0027 // The Port C Data Direction Register
        #define PORTC 0x0028 // The Port C Data Register
            #define PORTC6_PIN_1 6
            #define PORTC5_PIN_28 5
            #define PORTC4_PIN_27 4
            #define PORTC3_PIN_26 3
            #define PORTC2_PIN_25 2 
            #define PORTC1_PIN_24 1
            #define PORTC0_PIN_22 0
        #define PIND 0x0029 // The Port D Input Pins Address
        #define DDRD 0x002A // The Port D Data Direction Register
        #define PORTD 0x002B // The Port D Data Register
            #define PORTD7_PIN_13 7
            #define PORTD6_PIN_12 6
            #define PORTD5_PIN_11 5
            #define PORTD4_PIN_6 4
            #define PORTD3_PIN_5 3
            #define PORTD2_PIN_4 2 
            #define PORTD1_PIN_3 1
            #define PORTD0_PIN_2 0
        #define EIFR 0x003C
            #define INTF1 1
            #define INTF0 0
        #define TIFR0 0x0035
            #define TOV0 0 // Timer 0 Overflow Flag
        #define TIFR1 0x0036
        #define TIFR2 0x0037
        #define EIMSK 0x003D // External Interrupt Mask Register -> page 55
            #define INT0 0
            #define INT1 1
        #define GPIOR0 0x003E
        #define EECR 0x003F // EEPROM Control Register, only bit 0 to 5 is used, others must be aleays zero
            #define EERE_FLAG 0
            #define EEPE_FLAG 1
            #define EEMPE_FLAG 2
            #define EERIE_FLAG 3
            #define EEPM0_FLAG 4
            #define EEPM1_FLAG 5
        #define EEDR 0x040 // EEPROM Data Register, here goes the data to be written to EEPROM or data read from EEPROM
        #define EEARL 0x0041 // EEPROM Address Register Low Byte, only bit 0 and 1 is used, others must be aleays zero
        #define EEARH 0x0042 // EEPROM Address Register Low Byte
        #define TCNT0 0x0046 // Timer/Counter0 (8-bit)
        #define OCR0A 0x0047 // Timer/Counter0 output compare register A
        #define OCR0B 0x0048 // Timer/Counter0 output compare register B
        #define GPIOR1 0x004A
        #define GPIOR2 0x004B
        #define STACKPOINTER 0x005D
            #define SPL 0x005D
            #define SPH 0x005E
        #define SREG 0x005F //AVR Status Register -> ATmega datasheet page 20
            #define CARRY_FLAG 0 // C
            #define ZERO_FLAG 1 // Z
            #define NEGATIVE_FLAG 2 // N
            #define TWOS_COMPLEMENT_OVERFLOW_FLAG 3 // V
            #define SIGN_BIT 4 // S
            #define HALF_CARRY_FLAG 5 // H
            #define BIT_COPY_STORAGE 6 // T
            #define GLOBAL_INTERRUPT_ENABLE 7 // I
    #define IO_REGISTERS_END 0x005F
    #define EXTERN_IO_REGISTERS_START 0x0060
        #define EICRA 0x0069// External Interrupt Control Register A -> Datasheet page 54
            #define ISC00 0
            #define ISC01 1
            #define ISC10 2
            #define ISC11 3
        #define UBRR0L 0x00C4 // Datasheet page 276
        #define UBRR0H 0x00C5
        #define UDR0 0x00C6 // Serial Register
        #define BAD_ADDRESS 0x00F9
        #define CONSOLE_PRINT_HEX 0x00FA
        #define CONSOLE_PRINT_BIN 0x00FB
        #define CONSOLE_PRINT_CHAR 0x00FC
        #define CONSOLE_PRINT_DEC 0x00FD
        #define SIGNAL_EMULATOR 0x00FE
    #define EXTERN_IO_REGISTERS_END 0x00FF
    #define INTERNAL_SRAM_START 0x0100
    #define INTERNAL_SRAM_END 0x08FF
#define DATA_MEMORY_END 0x8FF

// EEPROM Data Memory Map
#define EEPROM_START 0x000
#define EEPROM_END 0x400
