#ifndef __HTTP_REQUEST__
#define __HTTP_REQUEST__
#include "./httpMethods.h"
#include "./ds.h"
#include "./httpVersion.h"
#include "../include/string.h"
#include <stdbool.h>

#define MAX_HEADERS 15
#define MAX_PARAMS 10


typedef struct HttpRequest {
  enum HttpMethod method;
  struct HttpVersion version;
  char *baseUri;
  struct Hashtable *headers;
  struct Hashtable *params;
  char *body;
} HttpRequest;

struct HttpRequest* createHttpRequest(void);
bool requestParser(String requestStr, struct HttpRequest* request);
void deleteHttpRequest(struct HttpRequest *request);
// For testing purpose
int selectHttpMethod(String methodStr);
bool headersParser(String *requestString, HttpRequest* request);
bool uriParser(String uri, HttpRequest *request);

#endif
