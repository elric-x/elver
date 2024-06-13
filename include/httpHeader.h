#ifndef __HTTP_HEADER__
#define __HTTP_HEADER__

typedef struct HttpHeader{
    char* name;
    char* value;
}HttpHeader;

void freeHeader(void* item);

#endif
