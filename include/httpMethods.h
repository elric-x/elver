#ifndef __HTTP_METHODS__
#define __HTTP_METHODS__

#define METHODS_COUNT 8

typedef enum HttpMethod{
    UNDEFINED = -1,
    GET,
    POST,
    DELETE,
    PUT,
    PATCH,
    STATIC,
}HttpMethod;


#endif
