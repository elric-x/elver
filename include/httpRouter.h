#ifndef __HTTP_ROUTER__
#define __HTTP_ROUTER__

#include "./hashtable.h"
#include "../include/httpRequest.h"
#include "../include/httpResponse.h"

#define MAX_ROUTES 5

typedef struct HttpRoute {
  enum HttpMethod method;
  char *uri;
  void (*middleware)(struct HttpRequest *request, struct HttpResponse *response);
} HttpRoute;

typedef struct HttpRouter {
  struct Hashtable *routes;
  struct HttpRouter* (*get)(struct HttpRouter *router, char *uri, void (*middleware)(struct HttpRequest *request, struct HttpResponse *response));
  struct HttpRouter* (*post)(struct HttpRouter *router, char *uri, void (*middleware)(struct HttpRequest *request, struct HttpResponse *response));
  struct HttpRouter* (*update)(struct HttpRouter *router, char *uri, void (*middleware)(struct HttpRequest *request, struct HttpResponse *response));
  struct HttpRouter* (*delete)(struct HttpRouter *router, char *uri, void (*middleware)(struct HttpRequest *request, struct HttpResponse *response));
} httpRouter;

struct HttpRoute* createRoute(enum HttpMethod method, char *uri, void (*middleware)(struct HttpRequest *request, struct HttpResponse *response));
struct HttpRouter* createRouter(void);
struct HttpRoute *selectRoute(struct HttpRouter *router, struct HttpRequest* request);
void deleteRouter(struct HttpRouter* router);
#endif
