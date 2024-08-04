#ifndef  __STRING__
#define __STRING__

#include <stdint.h>

typedef struct{
    const char *data;
    uint64_t len;
}String;

String chop_line(String *s);
String chop_word(String *s);
String chop_token(String *s, char c);
String trim_start(String s);
String trim_end(String s);
int str_find(String s, const char c);
char *str_flat(String s);
void str_printf(String s);

#endif
