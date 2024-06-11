#include <stdio.h>
#include "../include/log.h"

void logFn(enum Severity severity, char *message, char *filepath, int line) {
    time_t timestamp = time(NULL);
    char *logLevelMsg[3] = {"INFO", "WARNING", "ERROR"};
    if (severity == ERROR)
        fprintf(stderr, "%s\n\t[%s]: %s\n", asctime(localtime(&timestamp)), logLevelMsg[severity], message);
    else
        fprintf(stdout, "%s\n\t[%s]: %s\n", asctime(localtime(&timestamp)), logLevelMsg[severity], message);
}
