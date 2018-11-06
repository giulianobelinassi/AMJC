#include <graphviz/gvc.h>

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

Agnode_t* Type::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ZERO_CHILD_VERTEX(v, id);
    
    return v;
}
