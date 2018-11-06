#include <graphviz/gvc.h>
#include <iostream>

#include "macros.h"
#include "types.h"

Type::Type(const char* id)
{
    COPY_STR(this->id, id);
}

std::unordered_map<const char*, Type*> Type::registered_types;

Type* Type::getTypeFromStr(const char* id)
{
    Type* type;
    if (registered_types.find(id) == registered_types.end())
    {
        type = new Type(id);
        registered_types[id] = type;
    }
    else
        type = registered_types[id];

    return type;
}

Type* Type::getDeclaredType(const char* id)
{
    Type* type;
    if (registered_types.find(id) == registered_types.end())
        return NULL;
    else
        type = registered_types[id];

    return type;
}

Type* Type::declareType(const char* id)
{
    Type* type = NULL;
    if (registered_types.find(id) == registered_types.end())
    {
        type = new Type(id);
        registered_types[id] = type;
    }

    return type;
}

void Type::printDeclaredTypes()
{
    for (auto it = registered_types.begin(); it != registered_types.end(); ++it)
    {
        std::cout << "Declared Type: " << it->first << std::endl;
    }
}

Agnode_t* Type::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ZERO_CHILD_VERTEX(v, id);
    
    return v;
}
