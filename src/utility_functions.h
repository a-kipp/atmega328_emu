// All functions in this file are free from side effects. No global variables are used.
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


uint16_t uti_byteswap16bit(uint16_t num) {
    return num << 8 | num >> 8;
}

uint8_t uti_getBit(uint8_t byte, int bit) {
    bit = 1 << bit;
    return(bit & byte);
}

void uti_setBit(uint8_t *byte, int bit) {
    bit = 1 << bit;
    *byte = *byte | bit;
}

void uti_clearBit(uint8_t *byte, int bit) {
    bit = 1 << bit;
    bit ^= 0xff;
    *byte = *byte & bit;
}

uint32_t uti_extractBits(uint32_t num, int firstBit, int lastBit) {
    num >>= firstBit;
    uint32_t mask = 0xFFFFFF >> (16 - (lastBit - firstBit));
    return num ^= mask;
}


char *_getFileContent(FILE *file_ptr) {    
    int file_length = 1;
    char character = fgetc(file_ptr);
    
    while (fgetc(file_ptr) != EOF) {
        file_length++;
    };
    fseek(file_ptr, 0L, SEEK_SET);
    char *file_content = calloc(file_length, sizeof(char));
    
    for(int i = 0; i < file_length; i++) {
        file_content[i] = fgetc(file_ptr);
    }
    return file_content;
}


void _parse(uint16_t *memoryLocation_ptr, char *fileContent_ptr, int maxSize) {
    int fileSizeCounter = 0;
    while (*fileContent_ptr != EOF) {
        char startCharacter = 0;
        int byteCount = 0;
        int addressOffset = 0;
        int recordType = 0;
        sscanf(fileContent_ptr, "%1c%2x%4x%2x", &startCharacter, &byteCount, &addressOffset, &recordType);
        for(int i = 0; i < byteCount; i++) {
            printf("byteCount: %x\n", fileSizeCounter);
            int instruction = 0;
            if (fileSizeCounter > maxSize) {
                fprintf(stderr, "hex file too big, maximum memory size exceeded\n");
                return; 
                exit(1);
            }
            sscanf(fileContent_ptr, "%4x", &instruction);
            *memoryLocation_ptr = uti_byteswap16bit((uint16_t)instruction);
            memoryLocation_ptr++;
            fileSizeCounter++;
        }
        fileContent_ptr += 2;
    }
}


void uti_loadFile(uint16_t *memoryLocation_ptr, char *pathToFile, int maxSize) {
    FILE *file_ptr = fopen(pathToFile, "r");
    if(file_ptr == NULL) printf("can't open file\n");
    char *fileContent_ptr = _getFileContent(file_ptr);
    fclose(file_ptr);
    _parse(memoryLocation_ptr, fileContent_ptr, maxSize);
}