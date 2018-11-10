#include "symboltable.h"
#include "types.h"
#include "declaration.h"
#include "expression.h"

#include <iostream>

#define SYMBOL_INIT(t, i, v, ty, off) do { \
    this->type = (t); \
    this->id   = (i); \
    this->offset = (off); \
    this->val.as_##ty = (v); \
} while(0)

Symbol::Symbol(Type* type, std::string id, int val, int offset)
{
    SYMBOL_INIT(type, id, val, int, offset);
}

Symbol::Symbol(Type* type, std::string id, bool val, int offset)
{
    SYMBOL_INIT(type, id, val, bool, offset);
}
Symbol::Symbol(Type* type, std::string id, SymbolTable* val, int offset)
{
    SYMBOL_INIT(type, id, val, class, offset);
}

Symbol::Symbol(Type* type, std::string id)
{
    if (type->isBool())
        SYMBOL_INIT(type, id, val, bool, offset);
    else if (type->isInt())
        SYMBOL_INIT(type, id, val, int , offset);
    else if (type->isArr())
        SYMBOL_INIT(type, id, val, arr , offset);
    else 
        SYMBOL_INIT(type, id, val, class , offset);
}


SymbolTable::SymbolTable(std::list<ClassDecl*>* decls)
{
    std::list<ClassDecl*>::iterator it;

    for (it = decls->begin(); it != decls->end(); ++it)
    {
        std::string id = (*it)->name->token;
        Type* type = Type::getDeclaredType(id);
        
        Symbol symbol(type, id);
        table.push_back(symbol);
        std::cout << symbol.id << std::endl;
    }
}
