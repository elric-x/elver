#include "../include/utils.h"
#include <string.h>
#include <stdio.h>
#include <errno.h>

#ifndef SIZE_UNIT
#define KILO 1024
#define MEGA (1024 * KILO)
#endif

#define BUFFER_SIZE (10 * KILO)

char* readFile(char *file_path){
    char fileBuffer[BUFFER_SIZE] = {};
    FILE* filePtr = fopen(file_path, "r");

    if(!filePtr){
        fprintf(stderr, "Could not open the file %s: %s\n", file_path,strerror(errno));
        return NULL;
    }
    size_t readCount = fread(fileBuffer, sizeof(char), BUFFER_SIZE, filePtr);
    if(readCount == 0){
        fclose(filePtr);
        fprintf(stderr, "Could not open the file %s: %s\n", file_path,strerror(errno));
        return NULL;
    }
    fclose(filePtr);
    printf("len: %ld\n", strlen(fileBuffer));
    return strndup(fileBuffer, strlen(fileBuffer));
}
