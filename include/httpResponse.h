#ifndef __HTTP_RESPONSE__
#define __HTTP_RESPONSE__

#include "./queue.h"

typedef struct HttpResponse {
  int status_code;
  float version;
  struct Queue *headers;
  char *body;
} HttpResponse;

struct HttpResponse* createResponse(void);
char* parseResponse(struct HttpResponse* response);
void freeResponse(struct HttpResponse* response);

#endif
