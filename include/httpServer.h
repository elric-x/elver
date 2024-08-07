#ifndef __HTTP_SERVER__
#define __HTTP_SERVER__

#include "./httpRouter.h"
#include "./ds.h"
#include "./server.h"


typedef struct HttpServer {
  struct Server server;
  struct HttpRouter* router;
  char* staticDir;
} HttpServer;

struct HttpServer createHttpServer(int port, char *address);
void launchServer(struct HttpServer *http_server);
void staticMiddleware(struct HttpResponse *response, char* staticFilePath);

#endif
