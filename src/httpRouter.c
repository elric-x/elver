#include "../include/httpRouter.h"
#include <stdlib.h>
#include <string.h>
#include "../include/log.h"
#include "../include/log.h"
#include "../include/utils.h"

struct HttpRoute* createRoute(enum HttpMethod method, char *uri, void (*middleware)(struct HttpRequest *request, struct HttpResponse *response)) {
    struct HttpRoute *newRoute = (struct HttpRoute *)calloc(1, sizeof(struct HttpRoute));
    if (!newRoute) {
        logFn(ERROR, "Unable to allocate heap memory", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    newRoute->method = method;
    newRoute->middleware = middleware;
    copyString(&newRoute->uri, uri);
    return newRoute;
}

void deleteRoute(struct HttpRoute* route){
    free(route->uri);
    free(route);
    return;
}

// Router get method
struct HttpRouter* get(struct HttpRouter *router, char *uri, void (*middleware)(struct HttpRequest *request,struct HttpResponse *response)) {
    struct HttpRoute *newRoute = createRoute(GET, uri, middleware);
    char key[255] = "get_";
    strcat(key, uri);
    hashtableSet(router->routes, key, newRoute);
    return router;
}

// Router post method
struct HttpRouter* post(struct HttpRouter *router, char *uri, void (*middleware)(struct HttpRequest *request, struct HttpResponse *response)) {
    struct HttpRoute *newRoute = createRoute(POST, uri, middleware);
    char key[255] = "post_";
    strcat(key, uri);
    hashtableSet(router->routes, key, newRoute);
    return router;
}

// Router update method
struct HttpRouter* update(struct HttpRouter *router, char *uri, void (*middleware)(struct HttpRequest *request, struct HttpResponse *response)) {
    struct HttpRoute *newRoute = createRoute(UPDATE, uri, middleware);
    char key[255] = "update_";
    strcat(key, uri);
    hashtableSet(router->routes, key, newRoute);
    return router;
}

// Router delete method
struct HttpRouter* delete(struct HttpRouter *router, char *uri, void (*middleware)(struct HttpRequest *request, struct HttpResponse *response)) {
    struct HttpRoute *new_route = createRoute(DELETE, uri, middleware);
    char key[255];
    strcat(key, "delete_");
    strcat(key, uri);
    hashtableSet(router->routes, key, new_route);
    return router;
}
// End router methods

// Router creation function
struct HttpRouter* createRouter(void) {
    struct HttpRouter* newRouter = (struct HttpRouter*)calloc(1, sizeof(struct HttpRouter));
    if(!newRouter){
        logFn(ERROR, "Unable to allocate heap memory", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    newRouter->routes = hashtableCreate(MAX_ROUTES * 2, Custom);
    newRouter->get = get;
    newRouter->post = post;
    newRouter->update = update;
    newRouter->delete = delete;
    return newRouter;
}

struct HttpRoute *selectRoute(struct HttpRouter *router, struct HttpRequest* request) {
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
        case DELETE:
            strcpy(method_prefix, "delete_");
        break;
    }
    strcat(key, method_prefix);
    strcat(key, request->baseUri);
    struct Entry *foundEntry = hashtableGet(router->routes, key);
    if (!foundEntry) {
        logFn(WARNING, "URI not defined", __FILE__, __LINE__);
        // Select not found error page
        return NULL;
    }
    struct HttpRoute *FoundRoute = (struct HttpRoute *)foundEntry->value.Custom;
    return FoundRoute;
}

void deleteRouteImpl(void *item){
    struct HttpRoute* route = (struct HttpRoute*)item;
    deleteRoute(route);
    return;
}

void deleteRouter(struct HttpRouter* router) {
    hashtableDelete(router->routes, deleteRouteImpl);
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
