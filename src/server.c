#include "../include/server.h"
#include "../include/log.h"
#include <stdlib.h>

Server create_server(int domain, int type, int port, int address) {
  Server server;
  server.domain = domain;
  server.type = type;
  server.port = port;
  server.ip = address;

  server.socketfd = socket(server.domain, server.type, 0);
  if (server.socketfd == -1) {
    logFn(ERROR, "Unable to create server socket", __FILE__, __LINE__);
    exit(1);
  }
  server.addr.sin_family = server.domain;
  server.addr.sin_port = htons(server.port);
  server.addr.sin_addr.s_addr = htonl(server.ip);
  return server;
}
