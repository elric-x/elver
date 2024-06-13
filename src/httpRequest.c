#include "../include/httpRequest.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/hashtable.h"
#include "../include/log.h"
#include "../include/utils.h"


struct HttpRequest* createRequest(void){
    struct HttpRequest* newRequest = (struct HttpRequest*)calloc(1, sizeof(struct HttpRequest));
    if(!newRequest){
        logFn(CRITICAL, "Unable to allocate heap memory", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    newRequest->method = 0;
    newRequest->body = NULL;
    newRequest->params = NULL;
    newRequest->baseUri = NULL;
    newRequest->headers = NULL;
    newRequest->version = 0;

    return newRequest;
}
/* *************************** REQUEST PARSERS *****************************************************/
int selectHttpMethod(char *methodStr) {
    int method;
    if (strcmp(methodStr, "GET") == 0)
        method = GET;
    else if (strcmp(methodStr, "POST") == 0)
        method = POST;
    else if (strcmp(methodStr, "UPDATE") == 0)
        method = UPDATE;
    else if (strcmp(methodStr, "DELETE") == 0)
        method = DELETE;
    else {
        logFn(ERROR, "Unknown method", NULL, 0);
    }
    return method;
}


void httpRequestHeadersParser(char *rawHeaders, struct HttpRequest* request) {
    request->headers = hashtableCreate(MAX_HEADERS * 2, String);
    if(!request->headers){
        exit(EXIT_FAILURE);
    }
    char *headerLine = strtok(rawHeaders, "\r\n");
    do {
        char field[255], value[255];
        sscanf(headerLine, "%[^:]: %[^\r]", field, value);
        hashtableSet(request->headers, field, value);
        headerLine = strtok(NULL, "\r\n");
    } while (headerLine);
    return;
}

void httpUriParser(char *rawUri, struct HttpRequest *request) {
    if(!strchr(rawUri, '?')){
        copyString(&request->baseUri, rawUri);
        strcpy(request->baseUri, rawUri);
        return;
    }

    copyString(&request->baseUri, strtok(rawUri, "?"));
    struct Hashtable *params = hashtableCreate(MAX_PARAMS * 2, String);
    char *rawParams = strtok(NULL, "\r\n");
    char *param = strtok(rawParams, "&");
    do {
        char field[255], value[255];
        sscanf(param, "%[^=]=%[^\r]", field, value);
        hashtableSet(params, field, value);
        param = strtok(NULL, "&");
    } while (param);
    request->params = params;
    return;
}

void httpRequestParser(char *rawRequest, struct HttpRequest* request) {
    char requestString[strlen(rawRequest)+1];
    strcpy(requestString, rawRequest);

    for (int i = 0; i < strlen(requestString) - 4; i++) {
        if (requestString[i] == '\r' && requestString[i + 1] == '\n' &&
            requestString[i + 2] == '\r' && requestString[i + 3] == '\n') {
            requestString[i + 2] = '\n';
            requestString[i + 3] = '|';
        }
    }
    //Match request method string to the enun HttpMethod
    request->method = selectHttpMethod(strtok(requestString, " "));
    char *uri = strtok(NULL, " ");
    request->version = atof(strpbrk(strtok(NULL, "\r\n"), "/") + 1);
    char *rawHeaders = strtok(NULL, "|");
    // parse the body
    httpRequestHeadersParser(rawHeaders, request);
    httpUriParser(uri, request);
    return;
}

void freeRequest(struct HttpRequest *request){
    hashtableDelete(request->headers, NULL);
    if(request->params)
        hashtableDelete(request->params, NULL);
    free(request->baseUri);
    free(request);
    return;
}
