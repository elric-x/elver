#ifndef __HTTP_ROUTER__
#define __HTTP_ROUTER__

#include "./ds.h"
#include "../include/httpRequest.h"
#include "../include/httpResponse.h"


#define MAX_ROUTES 5

struct HttpRoute {
  enum HttpMethod method;
  char *uri;
  void (*middleware)(struct HttpRequest *request, struct HttpResponse *response);
};

struct HttpRouter {
    struct Hashtable *routes;
    char* staticRootPath;
    struct HttpRouter* (*get)(struct HttpRouter *router, const char *uri, void (*middleware)(struct HttpRequest *request, struct HttpResponse *response));
    struct HttpRouter* (*post)(struct HttpRouter *router, const char *uri, void (*middleware)(struct HttpRequest *request, struct HttpResponse *response));
    struct HttpRouter* (*update)(struct HttpRouter *router, const char *uri, void (*middleware)(struct HttpRequest *request, struct HttpResponse *response));
    struct HttpRouter* (*delete)(struct HttpRouter *router, const char *uri, void (*middleware)(struct HttpRequest *request, struct HttpResponse *response));
    struct HttpRouter* (*statik)(struct HttpRouter *router, const char *staticFilesRootPath, void (*middleware)(struct HttpRequest *request, struct HttpResponse *response));
    struct HttpRoute* (*findRoute)(struct HttpRouter *router, struct HttpRequest* request);
};

struct HttpRoute* createRoute(enum HttpMethod method, const char *uri, void (*middleware)(struct HttpRequest *request, struct HttpResponse *response));
struct HttpRouter* createRouter(void);
void deleteRouter(struct HttpRouter* router);
#endif
