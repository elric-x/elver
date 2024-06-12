#ifndef __QUEUE__
#define __QUEUE__
#include "./customType.h"

struct Node{
    union Data data;
    struct Node* next;
};
struct Queue{
    enum DataTypes _type;
    struct Node* head;
    struct Node* tail;
    unsigned int count;
};

struct Queue* queueCreate(enum DataTypes type);
void queuePush(struct Queue* queue, void* data);
void* queuePeek(struct Queue* queue);
void queuePop(struct Queue* queue, void (*freeCustomData)(void* item));
void queueDump(struct Queue* queue, void (*printCustomData)(void* item));
void queueDelete(struct Queue* queue, void (*freeData)(void* item));

#endif
