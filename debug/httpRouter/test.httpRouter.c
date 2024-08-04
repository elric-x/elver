#include "./test.httpRouter.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool assert(void* value, void* expectedResult, int (*cmpFn)(void* x, void*y)){
    if(cmpFn(value, expectedResult) == 0)
        return true;
    return false;
}


struct HttpRequest* createDummyRequest(enum HttpMethod method, char* uri){
    struct HttpRequest* dummyRequest = createHttpRequest();
    dummyRequest->method = method;
    strCpy(&dummyRequest->baseUri, uri);
    return dummyRequest;
}

void deleteDummyRequest(struct HttpRequest* dummyRequest){
    free(dummyRequest->baseUri);
    free(dummyRequest);
}

void dummyMiddleware(struct HttpRequest* request, struct HttpResponse* response){
    printf("hello from dummy request\n");
    return;
}

/* typedef struct HttpRoute {
  enum HttpMethod method;
  char *uri;
  void (*middleware)(struct HttpRequest *request, struct HttpResponse *response);
} HttpRoute; */

int cmpRoute(void* x, void* y){
    struct HttpRoute* routerX = (struct HttpRoute*)x;
    struct HttpRoute* routerY = (struct HttpRoute*)y;
    //Check if both of them are referencing the same memory address if not is on of them is NULL
    if(routerX == routerY)
        return 0;
    if(routerX == NULL && routerY != 0 || routerX != NULL && routerY == 0)
        return 1; 
    if(routerX->method == routerY->method && strcmp(routerX->uri, routerY->uri)== 0 && routerX->middleware == routerY->middleware)
        return 0;
    return 1;
}

bool test_createRoute(void){
    int passedTest = 0;
    int failedTest = 0;

    //invalid parameters
    {
        struct HttpRoute* newRoute = createRoute(GET, "/hello", dummyMiddleware);
        newRoute ? passedTest++: failedTest++;
        if(newRoute){
            free(newRoute->uri);
            free(newRoute);
        }
    }

    {
        struct HttpRoute* newRoute = createRoute(GET, "", dummyMiddleware);
        newRoute == NULL ? passedTest++: failedTest++;
        if(newRoute){
            free(newRoute->uri);
            free(newRoute);
        }
    }

    {
        struct HttpRoute* newRoute = createRoute(GET, "/hello", NULL);
        newRoute == NULL ? passedTest++: failedTest++;
        if(newRoute){
            free(newRoute->uri);
            free(newRoute);
        }
    }
    {
        struct HttpRoute* newRoute = createRoute(POST, "/hello", dummyMiddleware);
        newRoute != NULL ? passedTest++: failedTest++;
        if(newRoute){
            free(newRoute->uri);
            free(newRoute);
        }
    }

    printf("Passed: %d| Failed: %d\n", passedTest, failedTest);
    return failedTest == 0;
}

bool testRoute(struct HttpRouter *router, enum HttpMethod method, char* path, bool expectedResult){
    struct HttpRequest *dummyRequest = dummyRequest = createDummyRequest(method, path);
    struct HttpRoute* foundRoute = foundRoute = router->findRoute(router, dummyRequest);
    bool match = foundRoute? true: false;
    deleteDummyRequest(dummyRequest);
    return match == expectedResult;
}


bool test_createRouter(void){
    int passedTest = 0;
    int failedTest = 0;

    struct HttpRouter* router = createRouter();
    struct HttpRouter* returnedRouter = NULL;

    if(!router){
        printf("Unable to create Router\n");
        return false;
    }

    //Check the default value of the newly created router
    router->routes != NULL? passedTest++: failedTest++;
    router->staticRootPath == NULL? passedTest++: failedTest;
    router->get != NULL ? passedTest++: failedTest++;
    router->post != NULL ? passedTest++: failedTest++;
    router->update != NULL ? passedTest++: failedTest++;
    router->delete != NULL ? passedTest++: failedTest++;

    //Setup some dummy API routes
    returnedRouter = router->get(router, "/hello", dummyMiddleware);
    returnedRouter == router ? passedTest++: failedTest++;

    returnedRouter = router->post(router, "/hello", dummyMiddleware);
    returnedRouter == router ? passedTest++: failedTest++;

    returnedRouter = router->post(router, "/hello", dummyMiddleware);
    returnedRouter == router ? passedTest++: failedTest++;

    returnedRouter = router->delete(router, "/update", dummyMiddleware);
    returnedRouter == router ? passedTest++: failedTest++;


    struct HttpRequest *dummyRequest = NULL;
    struct HttpRoute* foundRoute = NULL;

    // Create a dummy GET request to the /hello path -> find
    testRoute(router, GET, "/hello", true) ? passedTest++: failedTest++;

    // Create a dummy GET request to the /hello path -> !find
    testRoute(router, GET, "/test", false) ? passedTest++: failedTest++;

    // Create a dummy GET request to the /index.html path -> !find static files have not been defined
    testRoute(router, GET, "/index.html", false) ? passedTest++: failedTest++;

    // Add static file path
    router->statik(router, "../../www",dummyMiddleware);
    // Create a dummy GET request to the /index.html path -> find 
    testRoute(router, GET, "/index.html", true) ? passedTest++: failedTest++;

    // Create a dummy GET request to the /styles/index.css path -> find 
    testRoute(router, GET, "/styles/index.css", true) ? passedTest++: failedTest++;

    // Create a dummy POST request to the /index.html path -> !find POST is not retrieving files from the server
    testRoute(router, POST, "/index.html", false) ? passedTest++: failedTest++;

    // Create a dummy POST request to the /hello path -> find     
    testRoute(router, POST, "/hello", true) ? passedTest++: failedTest++;

    deleteRouter(router);
    printf("Passed: %d| Failed: %d\n", passedTest, failedTest);
    return failedTest == 0;
}

bool test_findRoute(void){
    /* runTest(test_createRoute, "createRoute function");
    runTest(test_createRouter, "createRouter function"); */
    return false;
}
