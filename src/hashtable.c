#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../include/hashtable.h"
#include "../include/log.h"

//FNV1 hash algorithm
uint64_t hash(char* string){
    uint64_t hash = FNV_OFFSET;
    for(int i = 0; i < strlen(string); i++){
        hash *= FNV_PRIME;
        hash ^= string[i];
    }
    return hash;
}


struct Hashtable* hashtableCreate(int capacity, enum DataTypes type){
    struct Hashtable* hashtable = (struct Hashtable*)calloc(1, sizeof(struct Hashtable));
    struct Entry* slots[capacity];
    if(!hashtable){
        logFn(ERROR, "Unable to allocate heap memory", __FILE__, __LINE__);
        return NULL;
    }
    hashtable->entries = 0;
    hashtable->capacity = CAPACITY;
	hashtable->_type = type;
    for(int i = 0; i < CAPACITY; i++)
        hashtable->slots[i] = NULL;
    return hashtable;
}


struct Entry* createEntry(char* key, void* value, enum DataTypes type){
    struct Entry* newEntry = (struct Entry*)calloc(1, sizeof(struct Entry));
    if(!newEntry){
        fprintf(stderr, "%s:%d, Unable to allocate heap memory", __FILE__, __LINE__);
        return NULL;
    }

    newEntry->key = calloc(strlen(key) + 1, sizeof(char));
    if(!newEntry->key){
        free(newEntry);
        return NULL;
    }
    strcpy(newEntry->key, key);
    switch(type){
        case String:
            newEntry->value.String = (char*)calloc(strlen(value) + 1, sizeof(char));
            if(!newEntry->value.String){
                logFn(ERROR, "Index is out of range", __FILE__, __LINE__);
                free(newEntry);
                return NULL;
            }
            strcpy(newEntry->value.String, (char*)value);
        break;
        case Custom:
            newEntry->value.Custom = value;
        break;

    }
    newEntry->next = NULL;
    return newEntry;
}


void freeEntry(struct Entry* entry, enum DataTypes type,void (*deleteCustom)(void* value)){
    free(entry->key);
    if(type == Custom)
        deleteCustom(entry->value.Custom);
    else
        free(entry->value.String);
    free(entry);
    return;
}

bool hashtableSet(struct Hashtable* hashtable, char *key, void* value){
    uint64_t hashedKey = hash(key);
    int index = (int)(hashedKey & (uint64_t)(CAPACITY - 1));
    struct Entry* newEntry = createEntry(key, value, hashtable->_type);
    if(!newEntry)
        return false;
    if(!hashtable->slots[index]){
        hashtable->slots[index] = newEntry;
        hashtable->entries++;
        return true;
    }
    struct Entry* headEntry  = hashtable->slots[index];
    while(headEntry->next)
        headEntry = headEntry->next;
    headEntry->next = newEntry;
    return true;
}


struct Entry* hashtableGet(struct Hashtable* hashtable, char *key){
    uint64_t hashedKey = hash(key);
    int index = (int)(hashedKey & (uint64_t)(CAPACITY - 1));
    if(index > CAPACITY - 1){
        logFn(ERROR, "Index is out of range", __FILE__, __LINE__);
        return NULL;
    }
    struct Entry* headEntry = hashtable->slots[index];
    if(!headEntry){
        logFn(WARNING, "Key not found", NULL, 0);
            return NULL;
    }
    for(struct Entry* entry = headEntry; entry != NULL; entry = entry->next)
        if(strcmp(key, entry->key)==0)
            return entry;
    return NULL;
}

void hashtableDump(struct Hashtable* hashtable, void (*printCustomData)(void *data)){
    for(int i = 0; i < CAPACITY; i++){
        struct Entry* head = hashtable->slots[i];
        while(head){
            printf("%s:\n\t", head->key);

            if(hashtable->_type == String)
                printf("%s\n", head->value.String);
            else
                printCustomData(head->value.Custom);

            head = head->next;
        }
    }
    return;
}

//Need to be worked on
void hastableRemove(struct Hashtable* hashtable, char* key, void (*deleteCustom)(void* data)){
    int index = hash(key);
    if(index > CAPACITY - 1)
        logFn(ERROR, "Index is out of range", __FILE__, __LINE__);

    struct Entry* currentEntry  = hashtable->slots[index];
    if(!currentEntry)
        return;

    struct Entry* prevEntry, *nextEntry = NULL;
    while(currentEntry){
        nextEntry = currentEntry->next;
        if(strcmp(currentEntry->key, key) == 0)
            break;
        prevEntry = currentEntry;
        currentEntry = nextEntry;
    }
    /*
     * if prev = NULL 
     *     head->next = NULL then entry is the only entry on this slot so we delete the entry and the slot is now pointing to NULL
     *     head->next != NULL the entry was the head of the entry and the slot has to point now to the next entry as head
    */
    if(!prevEntry){
        if(!currentEntry->next)
            hashtable->slots[index] = NULL;
        else
            hashtable->slots[index] = currentEntry->next;
    }else{
        prevEntry->next = currentEntry->next;
    }
    freeEntry(currentEntry, hashtable->_type,deleteCustom);
}

void purgeSlot(struct Entry* head, enum DataTypes type, void (*deleteCustom)(void* data)){
    struct Entry* nextEntry = NULL;
    while(head){
        nextEntry = head->next;
        freeEntry(head, type, deleteCustom);
        head = nextEntry;
    }
    return;
}

void hashtableDelete(struct Hashtable* hashtable, void (*deleteCustom)(void* data)){
    for(int i = 0; i < CAPACITY; i++){
        purgeSlot(hashtable->slots[i], hashtable->_type, deleteCustom);	
        //hashtable->slots[i] = NULL;
    }
    free(hashtable);
    return;
}
