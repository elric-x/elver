#include <stdio.h>
#include "../include/log.h"

void logFn(enum Severity severity, char *message, char *filepath, int line) {
    time_t timestamp = time(NULL);
    char *logLevelMsg[3] = {"INFO", "WARNING", "ERROR"};
    if (severity == ERROR || severity == CRITICAL)
        fprintf(stderr, "%s\n[%s]\n%s:%d \t%s\n", asctime(localtime(&timestamp)), logLevelMsg[severity], filepath, line, message);
    else
        fprintf(stdout, "%s\n[%s]\n%s:%d \t%s\n", asctime(localtime(&timestamp)), logLevelMsg[severity], filepath, line, message);
}
