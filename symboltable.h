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
    SymbolTable* as_args;
};

class Symbol
{
    public:
    Type* type;
    std::string id;
    union container val;
    MethodDecl* func_body; // Or offset, when  compiled
    int offset;

    Symbol(Type*, std::string id);
    Symbol(Type*, std::string id, int  val);
    Symbol(Type*, std::string id, bool val);
    Symbol(Type*, std::string id, SymbolTable* val);
    Symbol(Type*, std::string id, SymbolTable* val, MethodDecl*);

};

class SymbolTable
{
    public:
    std::unordered_map<std::string, Symbol*> table;

    SymbolTable(Type*, ClassDecl*);
    SymbolTable(std::list<VarDecl*>*);

    void parseVars(std::list<VarDecl*>* vars);
    void parseMethods(std::list<MethodDecl*>* methods);

    inline Symbol* operator[](std::string key) {return table[key];}

    private:
    void checkIfAlreadyDeclared(std::string);
};

#endif
