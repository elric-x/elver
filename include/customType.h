#ifndef __CUSTOM_TYPE__
#define __CUSTOM_TYPE__

union Data{
    void* Custom;
    char* String;
};

enum DataTypes{
    Custom,
    String,
};

#endif
