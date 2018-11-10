#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "tree.h"

class Type;
class SymbolTable;


union container
{
    int as_int;
    bool as_bool;
    int* as_arr;
    SymbolTable* as_class;
//  FunctionDecl* func;
};

class Symbol
{
    public:
    Type* type;
    std::string id;
    union container val;
    int offset;

    Symbol(Type*, std::string id);
    Symbol(Type*, std::string id, int   val = 0   , int offset=0);
    Symbol(Type*, std::string id, bool  val = false, int offset=0);
    Symbol(Type*, std::string id, SymbolTable* val = NULL, int offset=0);
//    Symbol(Type*, id, FunctionDecl* val = NULL, int offset=0);

};

class SymbolTable
{
    public:
    std::unordered_map<std::string, Symbol> table;

    SymbolTable(std::list<ClassDecl*>*);
    SymbolTable(std::list<MethodDecl*>*);
    SymbolTable(std::list<VarDecl*>*);
};

#endif
