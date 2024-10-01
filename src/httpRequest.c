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
    //printf("%*s", (int)requestStr->len, (char*)requestStr->data);
    printf("Headers:\n");
    do {
        String headerLine = trim_end(chop_line(requestStr));
        String fieldStr = chop_token(&headerLine, ':');
        headerLine = trim_start(headerLine);
        struct HttpHeader *newHeader = calloc(1, sizeof(struct HttpHeader));
        if(!newHeader){
            logFn(ERROR, "Unable to create request header", __FILE__, __LINE__);
            return false;
        }

        newHeader->name = strndup(fieldStr.data, fieldStr.len);
        if(!newHeader->name){
            logFn(ERROR, "Unable to allocate memory for header name", __FILE__, __LINE__);
            free(newHeader);
            return false;
        }

        char name[fieldStr.len];
        if(!strcpy(name, newHeader->name)){
            logFn(ERROR, "Unable to copy header name", __FILE__, __LINE__);
            free(newHeader->name);
            free(newHeader);
            return false;
        }
        newHeader->value = strndup(headerLine.data, headerLine.len);
        if(!newHeader->value){
            logFn(ERROR, "Unable to allocate memory for header value", __FILE__, __LINE__);
            free(newHeader->name);
            free(newHeader);
            return false;
        }
        if(!hashtableSet(request->headers, name, newHeader)){
            logFn(ERROR, "Unable to append headers", __FILE__, __LINE__);
            hashtableDelete(request->headers, NULL);
            return false;
        }
    } while ((char)requestStr->data[0] != '\r');
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
        //printf("param: %.*s\n", (int)param.len, param.data);
        String fieldStr = chop_token(&param, '=');
        char *field = strndup(fieldStr.data, fieldStr.len);
        if(!field){
            logFn(ERROR, "Unable to create param name", __FILE__, __LINE__);
            hashtableDelete(request->params, deleteHttpHeaderWrapper);
            return false;
        }
        char *value = strndup(param.data, param.len);
        if(!value){
            logFn(ERROR, "Unable to create param value", __FILE__, __LINE__);
            hashtableDelete(request->params, deleteHttpHeaderWrapper);
            return false;
        }
        if(!hashtableSet(request->params, field, value)){
            logFn(ERROR, "Unable to create param", __FILE__, __LINE__);
            hashtableDelete(request->params, deleteHttpHeaderWrapper);
            return false;
        }
        //free(field);
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

void bodyParser(String rawBody, HttpRequest *request){
    struct Entry *entry = hashtableGet(request->headers, "Content-Type");
    if(!entry){
        request->body = NULL;
        return;
    }
    //char *bodyContenType = (char*) ((struct HttpHeader *)entry->value);
    (void)rawBody;
    return;
}

void printParam(void* data){
    printf("%s\n", (char*)data);
}

void dumpHttpRequest(struct HttpRequest* request){
    printf("Method: ");
    switch(request->method){
        case GET:
           printf("GET\n");
        break;
        case POST:
           printf("POST\n");
        break;
        case DELETE:
           printf("DELETE\n");
        break;
        case PUT:
           printf("PUT\n");
        break;
        case PATCH:
           printf("PATCH\n");
        break;
        case STATIC:
           printf("STATIC\n");
        break;
        default:
           printf("UNDEFINED\n");

    }
    printf("Version: %d.%d\n", request->version.major, request->version.minor);
    printf("base URI: %s\n", request->baseUri);
    hashtableDump(request->params, printParam);
    hashtableDump(request->headers, printHeader);
    printf("\n\n%s\n", (char*)request->body);
}

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
    headersParser(&requestString, request);
    //bodyParser();
    request->body = NULL;
    //dumpHttpRequest(request);
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


