#include "./test.httpRequest.h"
#include <stdio.h>
#include <string.h>

bool assert(void* value, void* expectedResult, int (*cmpFn)(void* x, void*y)){
    if(cmpFn(value, expectedResult) == 0)
        return true;
    return false;
}

bool test_createHttpRequest(void){
    struct HttpRequest* newRequest = createHttpRequest();
    if(!newRequest)
        return false;
    deleteHttpRequest(newRequest);
    return false;
}

bool test_selectHttpMethod(void){
    short passedTests = 0;
    short failedTest = 0;

    struct method{
        char methodStr[15];
        enum HttpMethod expectation;
    };
    struct method testCases[6] = {
        { .methodStr = "GET", .expectation = GET},
        { .methodStr = "POST", .expectation = POST},
        { .methodStr = "UPDATE", .expectation = UPDATE},
        { .methodStr = "DELETE", .expectation = DELETE},
        { .methodStr = "PATCH", .expectation = PATCH},
        { .methodStr = "UNKNOWN", .expectation = -1},

    };

    for(short i = 0; i < 6; i++){
        struct method testCase = testCases[i];
        selectHttpMethod(testCase.methodStr) == testCase.expectation? passedTests++: failedTest--;
    }
    return failedTest == 0;
}

void printHeader(struct HttpHeader* header){
    printf("%s: %s\n", header->name, header->value);
    return;
}

void printHeaderWrapper(void *item){
    if(!item)
        return;
    struct HttpHeader* header = (struct HttpHeader*)item;
    printHeader(header);
    return;
}

bool test_httpRequestHeaderParser(void){
    short passedTests = 0;
    short failedTest = 0;
    //Add more test cases
    char* testCases[]= {
        "Content-Type: application/json\r\nContent-Length: 81\r\nReferrer Policy:strict-origin-when-cross-origin\r\n",
        "Content-Type: text/html\r\nContent-Length: 81\r\n"
    };

    struct HttpRequest* newRequest = NULL;
    for(short i = 0; i < 2; i++){
        char testString[100];
        strcpy(testString, testCases[i]);
        newRequest = createHttpRequest();
        httpRequestHeadersParser(testString, newRequest);
        if(!newRequest->headers){
            failedTest++;
            deleteHttpRequest(newRequest);
        }else{
            hashtableDump(newRequest->headers, NULL );
            hashtableGet(newRequest->headers, "Content-Type")? passedTests++: failedTest++;
            hashtableGet(newRequest->headers, "Content-Length")? passedTests++: failedTest++;
            deleteHttpRequest(newRequest);
        }
    }

    return failedTest == 0;
}

bool test_httpUriParser(void){
    short testCaseCount = 0;
    char* testCases[]= {
    };
    struct HttpRequest* request = NULL;
    for(short i = 0; i < 0; i++){
        request = createHttpRequest();
        char testString[100];
        strcpy(testString, testCases[i]);
        httpUriParser(testString, request);
    }

    return false;
}

bool test_httpVersionParser(void){
    return false;
}

bool test_httpRequestParser(void){
    return false;
}
