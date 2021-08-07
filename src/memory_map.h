// Program Memory Map
#define PROAGRAM_MEMORY_START 0x000
    #define BOOT_SECTION_START
    #define BOOT_SECTION_END 0x7ff
#define PROGRAM_MEMORY_END 0x7ff

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
    #define EXTERN_IO_REGISTERS_END 0x00FF
    #define INTERNAL_SRAM_START 0x0100
    #define INTERNAL_SRAM_END 0x08FF
#define DATA_MEMORY_END 0x8FF

// EEPROM Data Memory Map
#define EEPROM_START 0x000
#define EEPROM_END 0x400