#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "tree.h"

class Type;
class SymbolTable;

#define ST_ARG      0
#define ST_LOCAL    1
#define ST_CLASS    2

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
    int where;

    Symbol(Type*);
    Symbol(Type*, int  val);
    Symbol(Type*, bool val);
    Symbol(Type*, SymbolTable* val);
    Symbol(Type*, SymbolTable* val, MethodDecl*);
    Symbol(Type*, int offset, int where);
    Symbol(Type*, SymbolTable* val, int offset, int where);
    Symbol(Type*, SymbolTable* val, MethodDecl*, int offset, int where);

};

class SymbolTable
{
    public:
    std::unordered_map<std::string, Symbol*> table;

    SymbolTable();
    SymbolTable(Type*, ClassDecl*);
    SymbolTable(std::list<VarDecl*>*);
    SymbolTable(SymbolTable*);

    void parseVars(std::list<VarDecl*>* vars, int where=ST_CLASS);
    void parseMethods(std::list<MethodDecl*>* methods);
    void printTable();

    inline Symbol* operator[](std::string key) {return table[key];}

    bool checkIfDeclared(std::string);
};

extern "C" {

void print_table(SymbolTable* st);

}

#endif
