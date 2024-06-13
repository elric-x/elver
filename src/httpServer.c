#include "../include/httpServer.h"
#include "../include/log.h"
#include "../include/utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

struct HttpServer createHttpServer(int port, char *address) {
    struct HttpServer httpServer = {};
    httpServer.server = create_server(AF_INET, SOCK_STREAM, port, INADDR_LOOPBACK);
    httpServer.router = createRouter();
    copyString(&httpServer.template_base_dir,
               "/home/dragonix/Dragonix/Sandbox/c/socket/www");
    return httpServer;
}

char *httpResquestHandler(struct HttpRequest *request, struct HttpResponse *response, httpRouter *router) {
    struct HttpRoute *foundRoute = selectRoute(router, request);
    if (!foundRoute) {
        logFn(INFO, "Route not found", __FILE__, __LINE__);
        char *not_found_message =
            "HTTP/1.0 404 NOT FOUND\r\n"
            "Server: webserver-c\r\n"
            "Content-type: text/html\r\n\r\n"
            "<html><h1>OUPSS this routes seems not to exists</h1><html>\r\n";
        char* parsed_response = NULL;
        copyString(&parsed_response, not_found_message);
        return parsed_response;
    } else {
        response->headers = queueCreate(Custom);
        if (!response->headers) {
            logFn(ERROR, "Unable to allocate heap memory", __FILE__, __LINE__);
            exit(1);
        }
        response->version = 1.1;
        foundRoute->middleware(request, response);
        char *parsed_response = parseResponse(response);
        /* freeResponse(response);
    freeRequest(request); */
        return parsed_response;
    }
}

void launchServer(struct HttpServer *httpServer) {
    int res = bind(httpServer->server.socketfd, (struct sockaddr *)&httpServer->server.addr, sizeof(httpServer->server.addr));
    if (res == -1) {
        logFn(ERROR, "Address already in use", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    res = listen(httpServer->server.socketfd, 5);
    if (res == -1) {
        logFn(ERROR, "Unable to start listening for connections", __FILE__,
              __LINE__);
        perror("error");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in clientAddr;
    int bufferSize = 30000 - 1;
    struct HttpRequest* request = NULL;
    struct HttpResponse* response = NULL;

    while (true) {
        request = createRequest();
        response = createResponse();

        int clientSocketFd = accept(httpServer->server.socketfd, NULL, NULL);
        if (clientSocketFd == -1) {
            logFn(ERROR, "Unable to accept client connection", __FILE__, __LINE__);
            exit(EXIT_FAILURE);
        }else {
            char buffer[30000];
            int readCharCount = read(clientSocketFd, buffer, bufferSize);
            if (readCharCount == -1) {
                logFn(ERROR, "Unable to read the client request", NULL, 0);
                exit(EXIT_FAILURE);
            }

            printf("New Connection... \n");
            httpRequestParser(buffer, request);
            char *output_msg = httpResquestHandler(request, response, httpServer->router);
            write(clientSocketFd, output_msg, strlen(output_msg));
            //free
            freeResponse(response);
            freeRequest(request);
            free(output_msg);

            close(clientSocketFd);
            printf("Connection Close \n");
        }
    }
    deleteRouter(httpServer->router);
    return;
}
