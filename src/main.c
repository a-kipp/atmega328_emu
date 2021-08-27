#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "emulator.h"


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
            case 'i': emu_init(); break;
            case 'l': emu_loadProgram(argv[1]); break;
            case 's': emu_start(); break;
            case 'h': emu_stop(); break;
            default: ; break;
        }
    }
}