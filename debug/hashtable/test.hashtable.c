#include <stdio.h>
#include <string.h>
#include "../../include/hashtable.h"
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

bool slotsEmpty(struct Hashtable* hashtable){
    for(int i = 0; i < hashtable->capacity; i++){
        struct Entry* headEntry = hashtable->slots[i];
        if(headEntry)
            return false;
    }
    return true;
}

bool test_hashTableCreate(){
    int totalTest = 4;
    int testPassed = 0;
    int testFailed = 0;
    
    enum DataTypes type = Custom;
    unsigned int capacity = 2;
    struct Hashtable* newHashtable = hashtableCreate(capacity, type);

    //Assigned data type
        newHashtable->_type == type ? testPassed++ : testFailed++;
    //Entries count
        newHashtable->entries == 0 ? testPassed++ : testFailed++;
    //Capacity
        newHashtable->capacity == capacity ? testPassed++ : testFailed++;
    //Slots empty
    slotsEmpty(newHashtable) ? testPassed++ : testFailed++;
    hashtableDelete(newHashtable, deleteHeader);
    return testFailed == 0;
}

bool test_hashtableSet(void){
    int totalTest = 1;
    int testPassed = 0;
    int testFailed = 0;

    {
        struct Hashtable* newHashtable = hashtableCreate(2, Custom);
        struct Header* header = createHeader("fullname", "Ouedraogo Eric Roland");
        if(!header){
            testFailed++;
        }
        hashtableSet(newHashtable, "k1016227", header);
        //hashtableDump(newHashtable,printHeader);
        hashtableDelete(newHashtable, deleteHeader);
    }    
    {
        struct Hashtable* newHashtable = hashtableCreate(2, String);
        hashtableSet(newHashtable, "K1016227", "Ouedraogo Wendinkonte Eric Roland");
        //hashtableDump(newHashtable, NULL);
        hashtableDelete(newHashtable, NULL);
    }


    return testFailed == 0;
}

void testHashtable(void){
    int totalTest = 1;
    int testPassed = 0;
    int testFailed = 0;

    printf("%shashtableCreate: \t", WHITE);
    test_hashTableCreate() ? printf("%sTest Passed\n", GREEN): printf("%sTest Failed\n", RED);
    printf("%shashtableSet:\t\t", WHITE);
    test_hashtableSet()? printf("%sTest Passed\n", GREEN): printf("%sTest Failed\n", RED);
    printf("%s", WHITE);
    return;
}
