#ifndef __LOG__
#define __LOG__
#include <stdbool.h>
#include <time.h>
#include "../include/stack.h"

enum Severity{
    INFO,
    WARNING,
    ERROR,
    CRITICAL,
};

struct Error{
    time_t _timestamp;
    enum Severity severity;
    char *filepath;
    int line;
    char *message;
    bool exit;
};



void logFn(enum Severity severity, char* message, char* filepath, int line);
#endif
