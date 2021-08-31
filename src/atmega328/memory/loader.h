#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


static char *_getFileContent(FILE *file_ptr) {    
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

// TODO make parser read more then 20 lines
static void _parseHexFile(uint16_t *programMemoryStart_ptr, char *fileContent_ptr, int maxSize) {
    for (int i = 0; i < 100; i++) {
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


// Public
// ____________________________________________________________________________________________________________________

void loa_loadHexFile(uint16_t *programMemoryStart_ptr, char *filePath, int maxSize) {
    FILE *file_ptr = fopen(filePath, "r");
    //if(file_ptr == NULL) printf("can't open file\n");
    char *fileContent_ptr = _getFileContent(file_ptr);
    fclose(file_ptr);
    _parseHexFile(programMemoryStart_ptr, fileContent_ptr, maxSize);
}