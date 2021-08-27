#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "atmega328.h"


int main(int argc, char *argv[] ) {

    bool is_running = true;

    if(argc != 2) {
        fprintf(stderr, "only one comandline argument allowed\n");
        exit(1);
    }

    while(is_running) {
        char character = getchar();
        switch (character) {
            case 'e': is_running = false; break;
            case 'i': atm_init(); break;
            case 'l': atm_loadProgram(argv[1]); break;
            case 's': atm_start(); break;
            case 'h': atm_stop(); break;
            default: ; break;
        }
    }
}