#include "../include/httpServer.h"
#include "../include/error.h"
#include "../include/utils.h"
#include "../include/httpHeader.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define KILO 1024
#define MEGA (1024 * KILO)

#define REQUEST_BUFFER_SIZE (5 * KILO)
char requestBuffer[REQUEST_BUFFER_SIZE] = {};
char static_dir[] = "/home/dragonix/Dragonix/Sandbox/c/socket/www";

void deleteHeaderWrapper(void* item){
    if(!item)
        return;
    struct HttpHeader* header = (struct HttpHeader*) item;
    deleteHeader(header);
    return;
}

struct HttpServer createHttpServer(int port, char *address) {
    (void)address;
    struct HttpServer httpServer = {};
    httpServer.server = create_server(AF_INET, SOCK_STREAM, port, INADDR_LOOPBACK);
    httpServer.router = createRouter();
    httpServer.staticDir = strndup(static_dir, strlen(static_dir));
    if(!httpServer.staticDir){
        close(httpServer.server.socketfd);
        exit(-1);
    }
    return httpServer;
}


void staticMiddleware(struct HttpResponse *response, char* staticFilePath) {
    char *fileExtension = strchr(staticFilePath, '.');
    if(!fileExtension){
        logFn(CRITICAL, "Unable to find file extension\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    fileExtension += 1;
    response->body = readFile(staticFilePath);

    if(strcmp(fileExtension, "html") == 0)
        setHeaderContentType(response->headers, HTML);
    else if(strcmp(fileExtension, "css") == 0)
        setHeaderContentType(response->headers, CSS);
    else if(strcmp(fileExtension, "js") == 0)
        setHeaderContentType(response->headers, JAVASCRIPT);
    else if(strcmp(fileExtension, "png") == 0)
        setHeaderContentType(response->headers, PNG);
    else if(strcmp(fileExtension, "jpeg") == 0)
        setHeaderContentType(response->headers, JPEG);
    else if(strcmp(fileExtension, "webp") == 0)
        setHeaderContentType(response->headers, WEBP);
    else if(strcmp(fileExtension, "json") == 0)
        setHeaderContentType(response->headers, JSON);
    else if(strcmp(fileExtension, "csv") == 0)
        setHeaderContentType(response->headers, CSV);
    response->body = readFile(staticFilePath);
    if (response->body)
        response->status_code = 200;
    else
        response->status_code = 500;
    return;
}

//TODO: Check this function

char *resquestHandler(struct HttpRequest *request, struct HttpResponse *response, struct HttpRouter *router) {
    struct HttpRoute *matchedRoute = router->findRoute(router, request);
    char *parsedResponse = NULL;
    response->headers = createQueue();
    if (!response->headers) {
        logFn(ERROR, "Unable to create response headers", __FILE__, __LINE__);
        return NULL;
    }

    if (!matchedRoute) {
        //if the the requested routes is not defined and it is a get method there is a statik method is used defined
        if(request->method == GET && router->staticRootPath){
            //compute the requested ressource path
            char filePath[256] = {};
            strcat(filePath, router->staticRootPath);
            strcat(filePath, request->baseUri);
            //check if such file exits
            if(access(filePath, F_OK) != 0){
                logFn(INFO, "Route not found", __FILE__, __LINE__);
                response->version = request->version;
                response->status_code = 404;
                //TODO: call push general header and add content-type
                char *notFoundMsg ="HTTP/1.0 404 NOT FOUND\r\n"
                    "Server: webserver-c\r\n"
                    "Content-type: text/html\r\n\r\n"
                    "<html><h1>OUPSS this routes seems not to exists</h1><html>\r\n";
                parsedResponse = strndup(notFoundMsg, strlen(notFoundMsg));
                if(!parsedResponse){
                    logFn(ERROR, "Unable to allocate heap memory", __FILE__, __LINE__);
                    return NULL;
                }
            }else{
                response->version = request->version;
                staticMiddleware(response, filePath);
                setGeneralHeaders(response->headers);
                dumpQueue(response->headers, printHeader);
                parsedResponse = parseResponse(response);
            }
        }else{
            char *notFoundMsg ="HTTP/1.0 404 NOT FOUND\r\n"
                "Server: webserver-c\r\n"
                "Content-type: text/html\r\n\r\n"
                "<html><h1>OUPSS this routes seems not to exists</h1><html>\r\n";
            parsedResponse = strndup(notFoundMsg, strlen(notFoundMsg));
        }

    } else {
        response->headers = createQueue();
        if (!response->headers) {
            logFn(ERROR, "Unable to create response headers", __FILE__, __LINE__);
            return NULL;
        }
        response->version = request->version;
        setGeneralHeaders(response->headers);
        matchedRoute->middleware(request, response);
        parsedResponse = parseResponse(response);
    }
    //free allocated response and request
    deleteQueue(response->headers, deleteHeaderWrapper);
    hashtableDelete(request->headers, deleteHeaderWrapper);

    if(request->baseUri)
        free(request->body);
    if(response->body)
        free(response->body);

    return parsedResponse;
}

void launchServer(struct HttpServer *httpServer){
    int res = bind(httpServer->server.socketfd, (struct sockaddr *)&httpServer->server.addr, sizeof(httpServer->server.addr));
    if (res == -1) {
        fprintf(stderr, "Could not bind the server socket: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    res = listen(httpServer->server.socketfd, 5);
    if (res == -1) {
        fprintf(stderr, "Could not listen to incomming request: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = 0;
    struct HttpRequest request = {.headers = NULL};
    struct HttpResponse response = {.body = NULL};

    while (true) {
        int clientfd = accept(httpServer->server.socketfd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (clientfd == -1) {
            fprintf(stderr, "Could not accept client request: %s", strerror(errno));
            continue;
        }

        int read_count = read(clientfd, requestBuffer, REQUEST_BUFFER_SIZE);
        //check if we are able to read the client request and if the request is not too log for the buffer
        if (read_count == -1) {
            fprintf(stderr, "Could not accept client request: %s", strerror(errno));
            continue;
        }
        String requestStr = {
            .len = read_count,
            .data = requestBuffer
        };

        printf("New Connection... \n");
        //parse the received request into an HttpRequest structure
        requestParser(requestStr, &request);
        //handle the request and return the formated response
        char *output_msg = resquestHandler(&request, &response, httpServer->router);
        write(clientfd, output_msg, strlen(output_msg));
        //free the request and response structure
        free(output_msg);
        //Close the connextion
        close(clientfd);
        printf("Connection Close \n");
    }
    deleteRouter(httpServer->router);
    return;
}
