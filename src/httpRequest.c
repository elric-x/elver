#include "../include/httpRequest.h"
#include "../include/httpHeader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/ds.h"
#include "../include/error.h"


struct HttpRequest* createHttpRequest(void){
    struct HttpRequest* newRequest = (struct HttpRequest*)calloc(1, sizeof(struct HttpRequest));
    if(!newRequest){
        logFn(CRITICAL, "Unable to allocate heap memory", __FILE__, __LINE__);
        return NULL;
    }
    return newRequest;
}
/* *************************** REQUEST PARSERS *****************************************************/

//convert the request method string to an enum value of HttpMethod
int selectHttpMethod(String methodStr) {
    enum HttpMethod method = UNDEFINED;
    if (memcmp(methodStr.data, "GET", methodStr.len) == 0)
        method = GET;
    else if (memcmp(methodStr.data, "POST", methodStr.len) == 0)
        method = POST;
    else if (memcmp(methodStr.data, "UPDATE", methodStr.len) == 0)
        method = UPDATE;
    else if (memcmp(methodStr.data, "DELETE", methodStr.len) == 0)
        method = DELETE;
    else if (memcmp(methodStr.data, "PUT", methodStr.len) == 0)
        method = PUT;
    else if (memcmp(methodStr.data, "PATCH", methodStr.len) == 0)
        method = PATCH;
    else 
        logFn(ERROR, "Unknown method", NULL, 0);
    return method;
}

void deleteHttpHeaderWrapper(void* item){
    if(!item)
        return;
    struct HttpHeader* header = (struct HttpHeader*) item;
    deleteHeader(header);
    return;
}

bool headersParser(String *requestStr, struct HttpRequest* request) {
    if(!request)
        return false;
    request->headers = hashtableCreate(MAX_HEADERS * 2);
    if(!request->headers){
        logFn(ERROR, "Unable to create headers", __FILE__, __LINE__);
        return false;
    }
    do {
        String headerLine = trim_end(chop_line(requestStr));
        String fieldStr = chop_token(&headerLine, ':');

        printf("\tfield: %.*s| value: %*.s\n", (int)fieldStr.len, fieldStr.data, (int)headerLine.len, headerLine.data);
        char *field = strndup(fieldStr.data, fieldStr.len);
        if(!field)
            return false;
        char *value = strndup(headerLine.data, headerLine.len);
        if(!value)
            return false;
        if(!hashtableSet(request->headers, field, value)){
            logFn(ERROR, "Unable to append headers", __FILE__, __LINE__);
            hashtableDelete(request->headers, NULL);
            return false;
        }
        free(field);
    } while (*requestStr->data != '\n');
    return true;
}


bool uriParser(String uri, HttpRequest *request) {
    // check for the ? in the URI -> parameters in the url
    if(!str_find(uri, '?')){
        request->baseUri = strndup(uri.data, uri.len);
        return request->baseUri ? true : false;
    }

   //extract the base uri 
    String baseUri = chop_token(&uri, '?');
    request->baseUri = strndup(baseUri.data, baseUri.len);
    // extract the params in the uri
    request->params = hashtableCreate(MAX_PARAMS * 2);
    if(!request->params){
        logFn(ERROR, "Unable to create params", __FILE__, __LINE__);
        free(request->baseUri);
        return false;
    }
    do {
        String param = chop_token(&uri, '&');
        printf("param: %.*s\n", (int)param.len, param.data);
        String fieldStr = chop_token(&param, '=');
        char *field = strndup(fieldStr.data, fieldStr.len);
        if(!field)
            return false;
        char *value = strndup(param.data, param.len);
        if(!value)
            return false;
        if(!hashtableSet(request->params, field, value)){
            logFn(ERROR, "Unable to create param", __FILE__, __LINE__);
            hashtableDelete(request->params, deleteHttpHeaderWrapper);
            return false;
        } 
        free(field);
    } while (uri.len > 0);
    return true;
}

void versionParser(String versionStr, HttpRequest *request){
    String majorStr = chop_token(&versionStr, '.');
    char *major = strndup(majorStr.data, majorStr.len);
    request->version.major = atoi(major);
    free(major);
    char *minor = strndup(versionStr.data, versionStr.len);
    request->version.minor = atoi(minor);
    free(minor);
    return;
}

// destructive action on rawRequest
bool requestParser(String requestString, struct HttpRequest* request) {
    String requestLine = chop_line(&requestString);
    //Match request method string to the enun HttpMethod
    request->method = selectHttpMethod(chop_word(&requestLine));
    if(request->method == UNDEFINED) 
        return false;
    String uri = chop_word(&requestLine);
    uriParser(uri, request);
    chop_token(&requestLine, '/');
    versionParser(requestLine, request);
    //headersParser(&requestString, request);
    //bodyParser();
    request->body = NULL;
    return true;
}

void deleteHttpRequest(struct HttpRequest *request){
    if(!request)
        return;
    if(request->headers)
        hashtableDelete(request->headers, deleteHttpHeaderWrapper);
    if(request->params)
        hashtableDelete(request->params, deleteHttpHeaderWrapper);
    free(request->baseUri);
    free(request);
    return;
}
