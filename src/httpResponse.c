#include "../include/httpResponse.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/error.h"
#include "../include/httpHeader.h"

#ifndef SIZE_UNIT
#define KILO 1024
#define MEGA (1024 * KILO)
#endif

#define BUFFER_SIZE (8 * KILO)

bool setHeaderContentType(Queue *headers, MimeType type){
    HttpHeader *header = NULL;
    switch(type){
        case PLAIN:
            header = createHeader("Content-Type", "text/plain");
        break;
        case HTML:
            header = createHeader("Content-Type", "text/html");
        break;
        case CSS:
            header = createHeader("Content-Type", "text/css");
        break;
        case JAVASCRIPT:
            header = createHeader("Content-Type", "text/javascript");
        break;
        case PNG:
            header = createHeader("Content-Type", "image/png");
        break;
        case JPEG:
            header = createHeader("Content-Type", "image/jpeg");
        break;
        case WEBP:
            header = createHeader("Content-Type", "image/webp");
        break;
        case JSON:
            header = createHeader("Content-Type", "application/json");
        break;
        case CSV:
            header = createHeader("Content-Type", "image/csv");
        break;
    }
    if(!header){
        fprintf(stderr, "Could not create HTTP header\n");
        return false;
    }
    pushQueue(headers, header);
    return true;
}

bool setGeneralHeaders(Queue *headers){
     // Buffer to hold the formatted date
    char buffer[80];
    // Get the current time
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%A, %d-%b-%y %H:%M:%S %Z", timeinfo);

    HttpHeader *header = createHeader("Server", "Elver");
    if(!header){
        fprintf(stderr, "Could not create HTTP header\n");
        return false;
    }
    pushQueue(headers, header);

    header = createHeader("Date", buffer);
    if(!header){
        fprintf(stderr, "Could not create HTTP header\n");
        return false;
    }
    pushQueue(headers, header);
    return true;
}

char* getStatusMessage(int status_code){
    char* message = NULL;
    switch(status_code){
        case 200:
            message = strndup("OK", strlen("OK"));
            if(!message){
                logFn(ERROR, "Unable to extract response message", __FILE__, __LINE__);
                return NULL;
            }
        break;
        case 201:
            message = strndup("CREATED", strlen("CREATED"));
            if(!message){
                logFn(ERROR, "Unable to extract response message", __FILE__, __LINE__);
                return NULL;
            }
        break;
        case 400:
            message = strndup("BAD REQUEST", strlen("BAD REQUEST"));
            if(!message){
                logFn(ERROR, "Unable to extract response message", __FILE__, __LINE__);
                return NULL;
            }
        break;
        case 401:
            message = strndup("UNAUTHORIZED", strlen("UNAUTHORIZED"));
            if(!message){
                logFn(ERROR, "Unable to extract response message", __FILE__, __LINE__);
                return NULL;
            }
        break;
        case 404:
            message = strndup("NOT FOUND", strlen("NOT FOUND"));
            if(!message){
                logFn(ERROR, "Unable to extract response message", __FILE__, __LINE__);
                return NULL;
            }
        break;
        default:
            message = strndup("NOT DEFINED", strlen("NOT DEFINED"));
            if(!message){
                logFn(ERROR, "Unable to extract response message", __FILE__, __LINE__);
                return NULL;
            }

    }
    return message;
}

// Add content length header to the header
char* parseResponse(HttpResponse *response){
    char* statusMessage = getStatusMessage(response->status_code);
    char responseBuffer[BUFFER_SIZE] = {};

    if(!statusMessage){
        logFn(ERROR, "Unable to create response message", __FILE__, __LINE__);
        return NULL;
    }

    //Header line
    char headerLine[KILO] ;
    if(sprintf(headerLine, "HTTP/%d.%d %d %s", response->version.major, response->version.minor, response->status_code, statusMessage) < 0){
        logFn(ERROR, "Unable to parse the response header line", __FILE__, __LINE__);
        return NULL;
    }
    free(statusMessage);
    
    //Headers
    char headers[KILO] = {};
    char parsedHeader[255] = {};
    while(true){
        //peek return the allocated memory on heap from the queue an then remove its address from it so the return address has to be freed
        struct HttpHeader* header = (struct HttpHeader*)peekQueue(response->headers);
        if(!header)
            break;
        sprintf(parsedHeader, "%s: %s\r\n", header->name, header->value);  
        strcat(headers, parsedHeader);
        deleteHeader(header);
    }
    int writtenCharCount = sprintf(responseBuffer, "%s\r\n%s\r\n%s", headerLine, headers, response->body);
    if(writtenCharCount == -1){
        fprintf(stderr, "Could not parse response\n");
        return NULL;
    }
    return strndup(responseBuffer, sizeof(responseBuffer));
}
