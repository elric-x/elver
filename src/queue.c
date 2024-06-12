#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/queue.h"
#include "../include/log.h"

/* union Data{
    void* Custom;
    char* String;
};

enum DataTypes{
    Custom,
    String,
}; */

struct Node* createNode(void* data, enum DataTypes type){
    struct Node* newNode = calloc(1, sizeof(struct Node));
    if(!newNode){
        logFn(ERROR, "Error Unable to allocate heap memory", __FILE__, __LINE__);
        return newNode;
    }
    switch(type){
        case String:
            newNode->data.String = (char*)calloc(strlen(data) + 1, sizeof(char));
            if(!newNode->data.String){
                logFn(ERROR, "Unable to allocate heap memory", __FILE__, __LINE__);
                free(newNode);
                return NULL;
            }
            strcpy(newNode->data.String, (char*)data);
        break;
        case Custom:
            newNode->data.Custom = data;
        break;
    }
    newNode->next = NULL;
    return newNode;
}

void freeNode(struct Node* node, enum DataTypes type, void (*freeCustomData)(void* data)){
    if(type == Custom)
        freeCustomData(node->data.Custom);
    else
        free(node->data.String);
    free(node);
    return;
}

struct Queue* queueCreate(enum DataTypes type){
    struct Queue* newQueue = (struct Queue*)calloc(1, sizeof(struct Queue));
    if(!newQueue){
        logFn(ERROR, "Error Unable to allocate heap memory", __FILE__, __LINE__);
        return NULL;
    }
    newQueue->_type = type;
    newQueue->head = NULL;
    newQueue->tail = NULL;
    newQueue->count = 0;
    return newQueue;
}


void queuePush(struct Queue* queue,  void* data){
    struct Node* newNode = createNode(data, queue->_type);
    if(!newNode){
        logFn(ERROR, "Error while creating the node", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
   
    if(queue->count == 0)
        queue->head = newNode;
    else
        queue->tail->next = newNode;

    queue->tail = newNode;
    queue->count++;

    return;
}

void* queuePeek(struct Queue* queue){
    void* data = NULL;
    struct Node* headNode = queue->head;

    if(queue->count == 0)
        return NULL;
    queue->head = headNode->next;
    if(!queue->head)
        queue->tail = NULL;

    switch(queue->_type){
        case String:
            data = headNode->data.String;
        break;
        case Custom:
            data = headNode->data.Custom;
        break;
    }
    free(headNode);
    queue->count--;
    return data;
}

void queuePop(struct Queue* queue, void (*freeCustomData)(void* item)){
    void* item = queuePeek(queue);
    freeCustomData(item);
    return;
}

void queueDump(struct Queue* queue, void (*printCustomData)(void* item)){
    if(!queue)
        return;
    struct Node* currentNode = queue->head;
    while(currentNode){
        if(queue->_type == String)
            printf("%s\n", currentNode->data.String);
        else
            printCustomData(currentNode->data.Custom);
        currentNode = currentNode->next;
    }
    return;
}

void queueDelete(struct Queue* queue, void (*freeCustomData)(void* item)){
    if(!queue)
        return;

    struct Node* currentNode = queue->head;
    struct Node* nextNode = NULL;
    while(currentNode){
        nextNode = currentNode->next;
        freeNode(currentNode, queue->_type, freeCustomData);
        currentNode = nextNode;
    }
    free(queue);
    return;
}

