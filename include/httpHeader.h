#ifndef __HTTP_HEADER__
#define __HTTP_HEADER__

typedef struct HttpHeader{
    char* name;
    char* value;
}HttpHeader;

struct HttpHeader* createHeader(char* name, char* value);
void deleteHeader(struct HttpHeader* header);
#endif
