#ifndef TYPES_H
#define TYPES_H

#include <unordered_map>

struct Agnode_s;
struct Agraph_s;

class Type
{
    public:
    static std::unordered_map<const char*, Type*> registered_types;
    static Type* getTypeFromStr(const char*);
    static Type* getDeclaredType(const char*);

    const char* id;
    
    struct Agnode_s* buildGVNode(struct Agraph_s*);

    Type(const char*);
};

#endif 
