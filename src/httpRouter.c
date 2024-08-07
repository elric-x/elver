#include "../include/httpRouter.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "../include/error.h"
#include "../include/utils.h"

struct HttpRoute* createRoute(HttpMethod method, const char *uri, void (*middleware)(HttpRequest *request, HttpResponse *response)) {
    if(!middleware || strlen(uri) == 0){
        logFn(WARNING, "middleware | uri undefined", __FILE__, __LINE__);
        return NULL;
    }
    struct HttpRoute *newRoute = (struct HttpRoute *)calloc(1, sizeof(struct HttpRoute));
    if (!newRoute) {
        logFn(ERROR, "Unable to allocate heap memory", __FILE__, __LINE__);
        return NULL;
    }
    //check if the method given by the user is correct -> HttpMethod
    newRoute->method = method;

    if(!middleware){
        logFn(ERROR, "Middleware should be defined", __FILE__, __LINE__);
        return NULL;
    }
    newRoute->middleware = middleware;
    newRoute->uri = strndup(uri, strlen(uri));
    if(!newRoute->uri){
        logFn(ERROR, "Unable to create new route", __FILE__, __LINE__);
        free(newRoute);
        return NULL;
    }
    return newRoute;
}

void deleteRoute(struct HttpRoute* route){
    if(route){
        free(route->uri);
        free(route);
    }else{
        logFn(WARNING, "Unable to create new route", __FILE__, __LINE__);
    }
    return;
}

// Router API route setup function
struct HttpRouter* get(struct HttpRouter *router, const char *uri, void (*middleware)(struct HttpRequest *request,struct HttpResponse *response)) {
    struct HttpRoute *newRoute = createRoute(GET, uri, middleware);
    if(!newRoute)
        logFn(ERROR, "Unable to register new route", __FILE__, __LINE__);
    char key[255] = "get_";
    strcat(key, uri);
    if(!hashtableSet(router->routes, key, newRoute))
        logFn(ERROR, "Unable to register new route", __FILE__, __LINE__);

    return router;
}

// Router post method
struct HttpRouter* post(struct HttpRouter *router, const char *uri, void (*middleware)(struct HttpRequest *request, struct HttpResponse *response)) {
    struct HttpRoute *newRoute = createRoute(POST, uri, middleware);
    if(!newRoute)
        logFn(ERROR, "Unable to register new route", __FILE__, __LINE__);

    char key[255] = "post_";
    strcat(key, uri);
    if(!hashtableSet(router->routes, key, newRoute))
        logFn(ERROR, "Unable to register new route", __FILE__, __LINE__);
    return router;
}

// Router update method
struct HttpRouter* update(struct HttpRouter *router, const char *uri, void (*middleware)(struct HttpRequest *request, struct HttpResponse *response)) {
    struct HttpRoute *newRoute = createRoute(UPDATE, uri, middleware);
    if(!newRoute)
        logFn(ERROR, "Unable to register new route", __FILE__, __LINE__);

    char key[255] = "update_";
    strcat(key, uri);
    if(!hashtableSet(router->routes, key, newRoute))
        logFn(ERROR, "Unable to register new route", __FILE__, __LINE__);
    return router;
}

// Router delete method
struct HttpRouter* delete(struct HttpRouter *router, const char *uri, void (*middleware)(struct HttpRequest *request, struct HttpResponse *response)) {
    struct HttpRoute *newRoute = createRoute(DELETE, uri, middleware);
    if(!newRoute)
        logFn(ERROR, "Unable to register new route", __FILE__, __LINE__);

    char key[255];
    strcat(key, "delete_");
    strcat(key, uri);
    if(!hashtableSet(router->routes, key, newRoute))
        logFn(ERROR, "Unable to register new route", __FILE__, __LINE__);
    return router;
}

// Router method for static routes
struct HttpRouter* statik(struct HttpRouter* router, const char* staticFilesRootPath){
    if(router->staticRootPath){
        logFn(ERROR, "A static folder has already been defined", __FILE__, __LINE__);
    }else{
        router->staticRootPath = strndup(staticFilesRootPath, strlen(staticFilesRootPath));
        if(!router->staticRootPath)
            logFn(ERROR, "Unable to register static route path", __FILE__, __LINE__);
    }
    return router;
}

/* struct HttpRouter* statik(struct HttpRouter* router, const char* staticFilesRootPath, void (*staticMiddleware)(struct HttpRequest *request, struct HttpResponse *response)){
    if(router->staticRootPath){
        logFn(ERROR, "A static folder has already been defined", __FILE__, __LINE__);
    }else{
        router->staticRootPath = strndup(staticFilesRootPath, strlen(staticFilesRootPath));
        if(!router->staticRootPath){
            logFn(ERROR, "Unable to register static route path", __FILE__, __LINE__);
        }else{
            struct HttpRoute* staticRoute = createRoute(STATIC, staticFilesRootPath, staticMiddleware);
            if(!staticRoute){
                logFn(ERROR, "Unable to register static route", __FILE__, __LINE__);
                free(router->staticRootPath);
            }else{
                if(!hashtableSet(router->routes, "static", staticRoute))
                    logFn(ERROR, "Unable to register new route", __FILE__, __LINE__);
            }
        }
    }
    return router;
} */
//Compute a file path from the request baseUri and the staticRootPath to check 
//if such ressource exits
//WARNING: Return false if it was not able to allocate space for the comparison
bool isRessource(char* staticRootPath, char* baseName){
    //compute the file path if it has to be in the static define folder
    char* computedFilePath = (char*)calloc(strlen(staticRootPath) + strlen(baseName) + 1, sizeof(char));
    if(!computedFilePath){
        logFn(ERROR, "Unable to allocate heap memory", __FILE__, __LINE__);
        return false;
    }
    strcat(computedFilePath, staticRootPath);
    strcat(computedFilePath, baseName);
    //
    //check if such file exits
    bool fileExists = access(computedFilePath, F_OK) != 0;
    free(computedFilePath);
    return fileExists;
}

struct HttpRoute* findRoute(struct HttpRouter *router, struct HttpRequest* request) {
    if(!request || !request->baseUri|| strlen(request->baseUri) == 0|| !router){
        if(!request)
            printf("request");
        if(!request->method)
            printf("method");
        if(!router)
            printf("router");
        logFn(ERROR, "Invalid parameters", __FILE__, __LINE__);
        return NULL;
    }

    char key[255] = {};
    char method_prefix[8] = {};
    switch(request->method){
        case GET:
            strcpy(method_prefix, "get_");
        break;
        case POST:
            strcpy(method_prefix, "post_");
        break;
        case UPDATE:
            strcpy(method_prefix, "update_");
        break;
        case PUT:
            strcpy(method_prefix, "put_");
        break;
        case PATCH:
            strcpy(method_prefix, "patch_");
        break;
        case DELETE:
            strcpy(method_prefix, "delete_");
        break;
        default:
        break;
    }
    strcat(key, method_prefix);
    strcat(key, request->baseUri);
    struct Entry *foundEntry = hashtableGet(router->routes, key);
    if(!foundEntry){
        /* printf("Blablablah\n");
        if(request->method == GET && router->staticRootPath  && isRessource(request->baseUri, router->staticRootPath))
            foundEntry = (struct Entry*) hashtableGet(router->routes, "static");
        else */
            return NULL;
    }
    struct HttpRoute *FoundRoute = (struct HttpRoute *)foundEntry->value;
    return FoundRoute;
}

// Router creation function
struct HttpRouter* createRouter(void) {
    struct HttpRouter* newRouter = (struct HttpRouter*)calloc(1, sizeof(struct HttpRouter));
    if(!newRouter){
        logFn(ERROR, "Unable to allocate heap memory", __FILE__, __LINE__);
        return NULL;
    }
    newRouter->routes = hashtableCreate(MAX_ROUTES * 2);
    if(!newRouter->routes){
        logFn(ERROR, "Unable to initialize routes", __FILE__, __LINE__);
        return NULL;
    }
    newRouter->staticRootPath = NULL;
    newRouter->get = get;
    newRouter->post = post;
    newRouter->update = update;
    newRouter->delete = delete;
    newRouter->statik = statik;
    newRouter->findRoute = findRoute;
    return newRouter;
}


void deleteRouteImpl(void *item){
    struct HttpRoute* route = (struct HttpRoute*)item;
    deleteRoute(route);
    return;
}

void deleteRouter(struct HttpRouter* router) {
    hashtableDelete(router->routes, deleteRouteImpl);
    if(router->staticRootPath)
        free(router->staticRootPath);
    free(router);
    return;
}

/*----------------------------------------------------------------------------------------------------------------------*/
/* void printRoute(void* data){
	struct httpRoute* route = (struct httpRoute*)data;
	printf("Method: %d | Uri: %s\n", route->method, route->uri);

}

void dumpRoutes(struct httpRouter *router){
	hashTableDump(&router->routes, printRoute);
} */
/*----------------------------------------------------------------------------------------------------------------------*/
