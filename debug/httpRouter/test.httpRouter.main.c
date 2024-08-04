#include "./test.httpRouter.h"
#include <stdio.h>

void runTest(bool (*testFn)(void), char *testName){
    printf("%s%s: \n", WHITE, testName);
    testFn()? printf("%sTest Passed\n", GREEN): printf("%sTest Failed\n", RED);
    printf("\n\n");
    return;
}

int main(void){
    runTest(test_createRoute, "createRoute function");
    runTest(test_createRouter, "createRouter function");
    return 0;
}
