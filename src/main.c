#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "atmega328/atmega328.h"
#include "atmega328/debug.h"


int main(int argc, char *argv[] ) {

    setvbuf (stdout, NULL, _IONBF, 0);

    bool is_running = true;

    if(argc != 2) {
        fprintf(stderr, "only one comandline argument allowed\n");
        exit(1);
    }
    
    atm_loadProgram(argv[1]);
    //atm_disassemble();
    atm_start();

    while(is_running) {
        char character = getchar();
        switch (character) {
            case 'e': is_running = false; break;
            case 'l': atm_loadProgram(argv[1]); break;
            case 's': atm_start(); break;
            case 'h': atm_stop(); break;
            case 'c': is_running = false; break;
            case 'm': deb_triggerSpeedTest(); break;
            case 'd': atm_disassemble(); break;
            case '1': atm_setPin5high(); break;
            case '0': atm_setPin5low(); break;
            default: ; break;
        }
    }
}