# compiler options
CC=avr-gcc
CFLAGS= -Os -DF_CPU=16000000L -mmcu=atmega328p

# compiling
all: blink.out

%.out: %.c
	$(CC) $(CFLAGS) $< -o $@

%.hex: %.out
	avr-objcopy -O ihex -R .eeprom $< $@
