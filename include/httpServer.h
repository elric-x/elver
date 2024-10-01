#ifndef __HTTP_SERVER__
#define __HTTP_SERVER__

#include "./httpRouter.h"
#include "./ds.h"
#include "./server.h"

#define HTTP_SERVER_DEFAULT_ADDRESS "127.0.0.1"
#define HTTP_SERVER_DEFAULT_PORT 8080

typedef struct HttpServer {
  struct Server server;
  struct HttpRouter* router;
  char* staticDir;
} HttpServer;

struct HttpServer createHttpServer(char *address, int port);
void launchServer(struct HttpServer *http_server);
void staticMiddleware(struct HttpResponse *response, char* staticFilePath);

#endif
