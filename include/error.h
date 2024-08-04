#ifndef __ERROR__
#define __ERROR__
#include <stdbool.h>
#include <time.h>

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
};


void throw(enum Severity severity, char* message, char* filePath, unsigned int lineNumber, void* errorStack);
//struct Error* catch(struct Severity severity, void* errorStack);
void logFn(enum Severity severity, char* message, char* filepath, int line);
#endif
