#include "../include/httpServer.h"
#include "../include/utils.h"

void homeMiddleware(struct HttpRequest *request, struct HttpResponse *response){
	char* file_path = "/home/dragonix/Dragonix/Sandbox/c/socket/www/index.html";
	response->body = readFile(file_path);
    if(response->body)
        response->status_code = 200;
    else
        response->status_code = 500;
	return;
}

void aboutMiddleware(struct HttpRequest *request, struct HttpResponse *response){
	char* file_path = "/home/dragonix/Dragonix/Sandbox/c/socket/www/about.html";
	response->body = readFile(file_path);
    if(response->body)
        response->status_code = 200;
    else
        response->status_code = 500;
	return;
}

void contactMiddleware(struct HttpRequest *request, struct HttpResponse *response){
	char* file_path = "/home/dragonix/Dragonix/Sandbox/c/socket/www/contact.html";
	response->body = readFile(file_path);
    if(response->body)
        response->status_code = 200;
    else
        response->status_code = 500;
	return;
}
/*----------------------------------------------------------------------------------------------------------------------*/
void testHttpServer(void) {
	//Server creation
	struct httpServer http_server = createHttpServer(8000, "192.168.1.1");

	//Route registration
	http_server.router
		.get(&http_server.router, "/", homeMiddleware)
		->get(&http_server.router, "/about", aboutMiddleware)
		->get(&http_server.router, "/contact", contactMiddleware)
	;
	//Start server
	launchServer(&http_server);

	return;
}
