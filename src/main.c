#include "../include/httpServer.h"
#include "../include/utils.h"
#include <stdio.h>

/************************************************************************8 */
void homeMiddleware(struct HttpRequest *request, struct HttpResponse *response) {
    (void)request;
    char *file_path = "/home/dragonix/Dragonix/Sandbox/c/socket/www/index.html";
    response->body = readFile(file_path);
    if (response->body)
        response->status_code = 200;
    else
        response->status_code = 500;
    return;
}

void aboutMiddleware(struct HttpRequest *request, struct HttpResponse *response) {
    (void)request;
    char *file_path = "/home/dragonix/Dragonix/Sandbox/c/socket/www/about.html";
    response->body = readFile(file_path);
    if (response->body)
        response->status_code = 200;
    else
        response->status_code = 500;
    return;
}

void contactMiddleware(struct HttpRequest *request, struct HttpResponse *response) {
    (void)request;
    printf("Contact route\n");
    char *file_path = "/home/dragonix/Dragonix/Sandbox/c/socket/www/contact.html";
    response->body = readFile(file_path);
    if (response->body)
        response->status_code = 200;
    else
        response->status_code = 500;
    return;
}

/*----------------------------------------------------------------------------------------------------------------------*/
int main(int argc, char **argv) {
    int serverPort = 0;
    if(argc < 2){
        serverPort = 8000;
    }else{
        serverPort = (int)atoi(argv[1]);
    }
    // Server creation
    struct HttpServer httpServer = createHttpServer(serverPort, "192.168.1.1");

    // Route registration
    httpServer.router
        ->get(httpServer.router, "/", homeMiddleware)
        ->get(httpServer.router, "/about", aboutMiddleware)
        ->get(httpServer.router, "/contact", contactMiddleware)
        ->statik(httpServer.router, "www");
    // Start server
    launchServer(&httpServer);
    return 0;
}
