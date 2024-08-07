#ifndef __HTTP_RESPONSE__
#define __HTTP_RESPONSE__

#include "./ds.h"
#include "httpVersion.h"

typedef struct HttpResponse {
  int status_code;
  struct HttpVersion version;
  struct Queue *headers;
  char *body;
} HttpResponse;

typedef enum{
    PLAIN,
    HTML,
    CSS,
    JAVASCRIPT,
    PNG,
    JPEG,
    WEBP,
    CSV,
    JSON,
}MimeType;

bool setGeneralHeaders(Queue *headers);
bool setHeaderContentType(Queue *headers, MimeType type);
struct HttpResponse* createResponse(void);
char* parseResponse(struct HttpResponse* response);
void freeResponse(struct HttpResponse* response);

#endif
