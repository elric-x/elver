#include "./test.httpRequest.h"
#include <stdio.h>

void runTest(bool (*testFn)(void), char *testName){
    printf("%s%s: \n", WHITE, testName);
    testFn()? printf("%sTest Passed\n", GREEN): printf("%sTest Failed\n", RED);
    printf("\n\n");
    return;
}

int main(void){
    runTest(test_selectHttpMethod, "Select Http method function");
    runTest(test_httpRequestHeaderParser, "Request Header parser function");
    return 0;
}
