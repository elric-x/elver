#include "../../include/queue.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../colors.h"

struct Header{
    char* name;
    char* value;
};


struct Header* createHeader(char* name, char* value){
    struct Header* newHeader = (struct Header*)calloc(1, sizeof(struct Header));
    if(!newHeader)
        return NULL;
    newHeader->name = (char*)calloc(strlen(name) + 1, sizeof(char));
    if(!newHeader->name){
        free(newHeader);
        return NULL;
    }
    strcpy(newHeader->name, name);
    newHeader->value = (char*)calloc(strlen(value) + 1, sizeof(char));
    if(!newHeader->value){
        free(newHeader);
        free(newHeader->name);
        return NULL;
    }
    strcpy(newHeader->value, value);
    return newHeader;
}

bool headerCmp(struct Header* x, struct Header* y){
    return strcmp(x->name, y->name) == 0 && strcmp(x->value, x->value) == 0;
}
void printHeader(void* data){
    struct Header* header = (struct Header*)data;
    printf("%s: %s\n", header->name, header->value);
    return;
}

void deleteHeader(void *item){
    struct Header *header = (struct Header*)item;
    free(header->name);
    free(header->value);
    free(header);
    return;
}

bool test_queueCreate(){
    int totalTest = 4;
    int testPassed = 0;
    int testFailed = 0;
    
    enum DataTypes type = Custom;
    struct Queue* newQueue = NULL;
    {
        type = String;
        newQueue = queueCreate(type);

        //Assigned data type
        newQueue->_type == type ? testPassed++ : testFailed++;
        //Entries count
        newQueue->count == 0 ? testPassed++ : testFailed++;
        //Head
        newQueue->head == NULL ? testPassed++ : testFailed++;
        //Tail
        newQueue->tail == NULL ? testPassed++ : testFailed++;

        queueDelete(newQueue, NULL);
    }

    {
        type = Custom;
        newQueue = queueCreate(type);

        //Assigned data type
        newQueue->_type == type ? testPassed++ : testFailed++;
        //Entries count
        newQueue->count == 0 ? testPassed++ : testFailed++;
        //Head
        newQueue->head == NULL ? testPassed++ : testFailed++;
        //Tail
        newQueue->tail == NULL ? testPassed++ : testFailed++;

        queueDelete(newQueue, deleteHeader);
    }
    return testFailed == 0;
}

bool test_queuePush(void){
    int totalTest = 4;
    int testPassed = 0;
    int testFailed = 0;
    
    enum DataTypes type = Custom;
    struct Queue* newQueue = NULL;
    {
        type = String;
        newQueue = queueCreate(type);
        char *fullname = "Ouedraogo Wendinkonte Eric";
        queuePush(newQueue, fullname);
        memcmp((char*)newQueue->head->data.String, fullname, strlen(fullname)) == 0 ? testPassed++ : testFailed++;
        newQueue->head == newQueue->tail ? testPassed++ : testFailed++;
        queueDelete(newQueue, NULL);
    }


    {
        type = Custom;
        newQueue = queueCreate(type);
        struct Header* newHeader = createHeader("fullname", "Ouedraogo Wendinkonte Eric");
        if(!newHeader){
            free(newQueue);
            return false;
        }
        queuePush(newQueue, newHeader);
        newQueue->head->data.Custom == newHeader ? testPassed++ : testFailed++;
        newQueue->head == newQueue->tail ? testPassed++ : testFailed++;
        queueDelete(newQueue, deleteHeader);
    }
    return testFailed == 0;
}

bool test_queuePeek(void){
    int totalTest = 4;
    int testPassed = 0;
    int testFailed = 0;
    
    enum DataTypes type = Custom;
    struct Queue* newQueue = NULL;
    {
        type = String;
        newQueue = queueCreate(type);
        char *string = "Ouedraogo Wendinkonte Eric";
        queuePush(newQueue, string);
        char *retrievedString = (char*) queuePeek(newQueue);

        strlen(string) == strlen(retrievedString) ? testPassed++ : testFailed++;
        strcmp(string, retrievedString) == 0 ? testPassed++ : testFailed++;
        newQueue->head == NULL ? testPassed++ : testFailed++;
        newQueue->tail == NULL ? testPassed++ : testFailed++;
        newQueue->count == 0 ? testPassed++ : testFailed++;

        free(retrievedString);
        queueDelete(newQueue, NULL);
    }

    {
        type = Custom;
        newQueue = queueCreate(type);
        struct Header* newHeader = createHeader("fullname", "Ouedraogo Wendinkonte Eric");
        if(!newHeader){
            free(newQueue);
            return false;
        }
        queuePush(newQueue, newHeader);
        struct Header* retrievedHeader = queuePeek(newQueue);
        headerCmp(newHeader, retrievedHeader) ? testPassed++ : testFailed++;
        newQueue->head == NULL ? testPassed++ : testFailed++;
        newQueue->tail == NULL ? testPassed++ : testFailed++;
        newQueue->count == 0 ? testPassed++ : testFailed++;
        deleteHeader(retrievedHeader);
        queueDelete(newQueue, deleteHeader);
    }
    return testFailed == 0;
}

void testQueue(void){
    printf("%squeueCreate:\t", WHITE);
    test_queueCreate() ? printf("%sTest Passed\n", GREEN) : printf("%sTest Failed\n", RED);
    printf("%squeuePush:\t", WHITE);
    test_queuePush() ? printf("%sTest Passed\n", GREEN) : printf("%sTest Failed\n", RED);
    printf("%squeuePeek:\t", WHITE);
    test_queuePeek()? printf("%sTest Passed\n", GREEN) : printf("%sTest Failed\n", RED);
    printf("%s", WHITE);
    return;
}
