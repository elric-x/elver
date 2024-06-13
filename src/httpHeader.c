#include "../include/httpHeader.h"
#include <stdlib.h>

void freeHeader(void* item){
    struct HttpHeader* header = (struct HttpHeader*)item;
    free(header->name);
    free(header->value);
    free(header);
}

