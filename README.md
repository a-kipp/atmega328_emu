An ATmega328P Emulator

A yet unfinished emulator for the ATmega328P wich is found in Arduiono.
The emulator supports the most common instructions and some of the interrupts.
In order to speed things up, I experimented with some optimization such as
using jumptables for the 16-bit instruction decoding. I can run the emulator at
about 200 MHz on my machine (Arduino runs at 16 Mhz). The emulator core is
written in pure C and can be ported to embedded devices.

