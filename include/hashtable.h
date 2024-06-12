#ifndef __HASH_TABLE__
#define __HASH_TABLE__
#include <stdbool.h>
#include <stdlib.h>
#include "./customType.h"

#define CAPACITY 10 
#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL


struct Entry{
    char* key;
    union Data value;
  //  enum DataTypes type;
    struct Entry* next;
};

struct Hashtable{
    enum DataTypes _type;
    unsigned int entries;
    unsigned int capacity;
    bool emptySlot;
    struct Entry** slots;
};


struct Hashtable* hashtableCreate(unsigned int capacity, enum DataTypes type);
bool hashtableSet(struct Hashtable* hashtable, char *key, void* value);
struct Entry* hashtableGet(struct Hashtable* hashtable, char *key);
void hashtableDump(struct Hashtable* hashtable, void (*printData)(void *data));
void hastableRemove(struct Hashtable* hashtable, char* key, void (*deleteCustom)(void* data));
void hashtableDelete(struct Hashtable* hashtable, void (*deleteCustom)(void* data));
#endif
