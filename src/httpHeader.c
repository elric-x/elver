#include "../include/httpHeader.h"
#include "../include/error.h"
#include <stdlib.h>
#include <string.h>

struct HttpHeader* createHeader(char* name, char* value){
    struct HttpHeader* newHeader = calloc(1, sizeof(struct HttpHeader));
    if(!newHeader){
        logFn(CRITICAL, "Unable to allocate heap memory", __FILE__, __LINE__);
        return NULL;
    }
    newHeader->name = strndup(name, strlen(name));
    if(!newHeader->name){
        logFn(CRITICAL, "Unable to allocate heap memory", __FILE__, __LINE__);
        free(newHeader);
        return NULL;
    }

    newHeader->value = strndup(value, strlen(value));
    if(!newHeader->value){
        logFn(CRITICAL, "Unable to allocate heap memory", __FILE__, __LINE__);
        free(newHeader->name);
        free(newHeader);
        return NULL;
    }
    return newHeader;
}


void deleteHeader(struct HttpHeader* header){
    if(!header)
        return;
    free(header->name);
    free(header->value);
    free(header);
}

