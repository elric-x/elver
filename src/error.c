#include <stdio.h>
#include <stdlib.h>
#include "../include/error.h"

void logFn(enum Severity severity, char *message, char *filepath, int line) {
    time_t timestamp = time(NULL);
    char *logLevelMsg[3] = {"INFO", "WARNING", "ERROR"};
    if (severity == ERROR || severity == CRITICAL)
        fprintf(stderr, "%s[%s]\n%s:%d \t%s\n", asctime(localtime(&timestamp)), logLevelMsg[severity], filepath, line, message);
    else
        fprintf(stdout, "%s[%s]\n%s:%d \t%s\n", asctime(localtime(&timestamp)), logLevelMsg[severity], filepath, line, message);
}

void throw(enum Severity severity, char* message, char* filePath, unsigned int lineNumber, void* errorStack){
    //create a new error
    (void)errorStack;
    logFn(severity, message, filePath, lineNumber);
    if(severity == CRITICAL)
        exit(EXIT_FAILURE);
    /* if(severity != CRITICAL){
        struct Error* newError = createError(Severity, char* message, char* filePath, int line);
        if(!newError){
            logFn(CRITICAL, "Unable to register the thrown error", __FILE__, __LINE__);
            //delete the error stack
            exit(EXIT_FAILURE);
        }
        //register the thrown error in the stack 
    }else{
        exit(EXIT_FAILURE)
    } */
}
//struct Error* catch(struct Severity severity, void errorStack);

