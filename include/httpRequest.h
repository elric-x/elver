#ifndef __HTTP_REQUEST__
#define __HTTP_REQUEST__
#include "./httpMethods.h"

#define MAX_HEADERS 15
#define MAX_PARAMS 10

typedef struct HttpRequest {
  enum HttpMethod method;
  float version;
  char *baseUri;
  struct Hashtable *headers;
  struct Hashtable *params;
  char *body;
} HttpRequest;

struct HttpRequest* createRequest(void);
void httpRequestParser(char *rawRequest, struct HttpRequest* request);
void freeRequest(struct HttpRequest *request);

#endif
