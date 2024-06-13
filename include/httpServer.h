#ifndef __HTTP_SERVER__
#define __HTTP_SERVER__

#include "./httpRouter.h"
#include "./queue.h"
#include "./server.h"


typedef struct HttpServer {
  struct Server server;
  struct HttpRouter* router;
  char* template_base_dir;
} HttpServer;

struct HttpServer createHttpServer(int port, char *address);
void launchServer(struct HttpServer *http_server);

#endif
