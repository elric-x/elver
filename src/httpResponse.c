#include "../include/httpResponse.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/log.h"
#include "../include/httpHeader.h"
#include "../include/utils.h"

struct HttpResponse* createResponse(void){
    struct HttpResponse* newResponse = (struct HttpResponse*)calloc(1, sizeof(struct HttpResponse));
    if(!newResponse){
        logFn(CRITICAL, "Unable to allocate heap memory", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    newResponse->headers = NULL;
    newResponse->body = NULL;
    return newResponse;
}

char* getStatusMessage(int status_code){
    char* message = NULL;
    switch(status_code){
        case 200:
            copyString(&message, "OK");
        break;
        case 201:
            copyString(&message, "CREATED");
        break;
        case 400:
            copyString(&message, "BAD REQUEST");
        break;
        case 401:
            copyString(&message, "UNAUTHORIZED");
        break;
        case 404:
            copyString(&message, "NOT FOUND");
        break;
    }
    return message;
}

// Add content length header to the header
char* parseResponse(struct HttpResponse* response){
    char* status_message = getStatusMessage(response->status_code);
    //Header line
    char header_line[100] ;
    if(sprintf(header_line, "HTTP/%.1f %d %s", response->version, response->status_code, status_message) < 0){
        logFn(ERROR, "Unable to parse the response header line", __FILE__, __LINE__);
        exit(1);
    }
    free(status_message);

    //Headers
    char headers[4000];
    char * parsed_header = NULL;
    while(true){
        //peek return the allocated memory on heap from the queue an then remove its address from it so the return address has to be freed
        struct HttpHeader* header = (struct HttpHeader*)queuePeek(response->headers);
        if(!header)
            break;
        parsed_header = (char*)calloc(strlen(header->name) + strlen(header->value) + 1, sizeof(char));
        sprintf(parsed_header, "%s: %s", header->name, header->value);  
        strcat(headers, parsed_header);
        freeHeader(header);
    }
    unsigned int response_size = strlen(header_line) + strlen(response->body) + strlen(headers) + 1;
    char* parsed_response = (char*)calloc(response_size, sizeof(char));
    sprintf(parsed_response, "%s\r\n%s\r\n\r\n%s", header_line, headers, response->body);
    return parsed_response;
}

void freeResponse(struct HttpResponse* response){
    queueDelete(response->headers, freeHeader);
    free(response->body);
    free(response);
    return;
}
