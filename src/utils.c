#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/log.h"
#include "../include/utils.h"

#define READ_BUFF 200000

bool copyString(char** destination, char* source){
    *destination = calloc(strlen(source) + 1, sizeof(char));
    if(!destination){
        fprintf(stderr, "%s:%d, Unable to allocate heap memory", __FILE__, __LINE__);
        return false;
    }
    strcpy(*destination, source);
    return true;
}

char* readFile(char *file_path){
    FILE* file_ptr = fopen(file_path, "r");
    if(!file_ptr){
        logFn(ERROR, "Unable to open the file", __FILE__, __LINE__);
        return NULL;
    }
    char* file_content = (char*)calloc(READ_BUFF, sizeof(char));
    if(!file_content){
        logFn(ERROR, "Unable to allocate heap for reading the file file", __FILE__, __LINE__);
        fclose(file_ptr);
        return NULL;
    }
    char line[200]; 

     while(fgets(line, 100, file_ptr))
        strcat(file_content, line);
    fclose(file_ptr);
    return file_content;
}

/*
void pwd(char* directory[75]){
	char* path = __FILE__;
	int pos = 0;
	for(int i = 0; i < strlen(path); i++)
		if(path[i] == '/')
			pos = i;
	char subString[75] = {};
	int i = 0;
	while(i <= pos)
		subString[i] = path[i];
	subString[pos+1] = '\0';
	strcpy(*directory, subString);
	return;
})
 */
