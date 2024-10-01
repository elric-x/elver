#ifndef __SERVER__
#define __SERVER__
#include <arpa/inet.h>
#include <unistd.h>

typedef struct Server{
    int domain;
    int type;
    int port;
    char address[15];
    int socketfd;
    struct sockaddr_in addr;
}Server;

Server create_server(int domain, int type, char* address, int port);
#endif
