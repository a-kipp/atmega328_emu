#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "memory.h"


int main(int argc, char *argv[] ) {
    if(argc != 2) {
        fprintf(stderr, "only one comandline argument allowed\n");
        exit(1);
    }

    mem_init(argv[1]);
}