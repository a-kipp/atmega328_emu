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


void _parse(uint16_t *programMemoryStart_ptr, char *fileContent_ptr, int maxSize) {
    for (int i = 0; i < 4; i++) {
        fileContent_ptr+=1;
        char byteCountBuffer[3] = {0};
        byteCountBuffer[0] = *fileContent_ptr; fileContent_ptr++;
        byteCountBuffer[1] = *fileContent_ptr; fileContent_ptr++;
        int byteCount = strtol(byteCountBuffer, NULL, 16);
        char addressOffsetBuffer[5] = {0};
        addressOffsetBuffer[0] = *fileContent_ptr; fileContent_ptr++;
        addressOffsetBuffer[1] = *fileContent_ptr; fileContent_ptr++;
        addressOffsetBuffer[2] = *fileContent_ptr; fileContent_ptr++;
        addressOffsetBuffer[3] = *fileContent_ptr; fileContent_ptr++;
        int addressOffset = strtol(addressOffsetBuffer, NULL, 16);
        uint16_t *instruction_ptr = programMemoryStart_ptr + addressOffset/2;
        fileContent_ptr += 2;
        for(int i = 0; i < byteCount; i+=2) {
            char instructionBuffer[5] = {0};
            instructionBuffer[0] = *fileContent_ptr; fileContent_ptr++;
            instructionBuffer[1] = *fileContent_ptr; fileContent_ptr++;
            instructionBuffer[2] = *fileContent_ptr; fileContent_ptr++;
            instructionBuffer[3] = *fileContent_ptr; fileContent_ptr++;
            int instruction = strtol(instructionBuffer, NULL, 16);
            *instruction_ptr = (uint16_t)instruction;
            instruction_ptr++;
        }
        fileContent_ptr += 4;
    }
}


void uti_loadFile(uint16_t *programMemoryStart_ptr, char *pathToFile, int maxSize) {
    FILE *file_ptr = fopen(pathToFile, "r");
    if(file_ptr == NULL) printf("can't open file\n");
    char *fileContent_ptr = _getFileContent(file_ptr);
    fclose(file_ptr);
    _parse(programMemoryStart_ptr, fileContent_ptr, maxSize);
}