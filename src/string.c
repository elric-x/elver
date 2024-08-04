#include "../include/string.h"
#include <stdint.h>
#include <string.h>
#include <ctype.h>
// Construct String from NULL-terminated C string
static inline
String str_create(const char *s){
    String result = {
        .len = strlen(s),
        .data = s
    };
    return result;
}

static inline
String str_empty(void)
{
    String result = {
        .len = 0,
        .data = NULL
    };
    return result;
}

int str_find(String s, const char c){
    uint64_t i = 0;
    while(i < s.len){
        if(s.data[i] == c)
            return 1;
        i++;
    }
    return 0;
}

String chop_token(String *input, char delim)
{
    if (input->len == 0) {
        return str_empty();
    }

    uint64_t i = 0;
    while (i < input->len && input->data[i] != delim)
        ++i;

    String line;
    line.data = input->data;
    line.len = i;

    if (i == input->len) {
        input->data += input->len;
        input->len = 0;
    } else {
        input->data += i + 1;
        input->len -= i + 1;
    }

    return line;
}

String chop_line(String *input) {
    return chop_token(input, '\n');
}

String trim_start(String s)
{
    while (s.len && isspace(*s.data)) {
        s.data++;
        s.len--;
    }
    return s;
}

String trim_end(String s)
{
    while (s.len && isspace(s.data[s.len - 1])) {
        s.len--;
    }
    return s;
}

String chop_word(String *input)
{
    // TODO: is this guard even needed?
    if (input->len == 0) {
        return str_empty();
    }

    uint64_t i = 0;
    while (i < input->len && !isspace(input->data[i])) {
        ++i;
    }

    String word;
    word.data = input->data;
    word.len = i;
    if(i == input->len){
        input->data += input->len;
        input->len = 0;
    }else{
        input->data += i + 1;
        input->len -= i + 1;
    }


    return word;
}

int string_equal(String a, String b)
{
    if (a.len != b.len) return 0;
    return memcmp(a.data, b.data, a.len) == 0;
}
