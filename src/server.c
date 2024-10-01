#include "../include/server.h"
#include "../include/error.h"
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Server create_server(int domain, int type, char* address, int port) {
    printf("%s\n", address);
    Server server;
    server.domain = domain;
    server.type = type;
    server.port = port;
    if(strcpy(server.address, address) == NULL){
        fprintf(stderr, "Error while setting server IP address" );
        exit(1);
    }

    server.socketfd = socket(server.domain, server.type, 0);
    if (server.socketfd == -1) {
        logFn(ERROR, "Unable to create server socket", __FILE__, __LINE__);
        exit(1);
    }
    server.addr.sin_family = server.domain;
    server.addr.sin_port = htons(server.port);
    server.addr.sin_addr.s_addr = inet_addr(server.address);
    return server;
}
