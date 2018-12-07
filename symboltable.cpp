#include "symboltable.h"
#include "types.h"
#include "declaration.h"
#include "expression.h"

#include <iostream>

#define SYMBOL_INIT(t, i, v, ty, off, f) do { \
    this->type = (t); \
    this->id   = (i); \
    this->offset = (off); \
    this->val.as_##ty = (v); \
    this->func_body = (f); \
} while(0)

Symbol::Symbol(Type* type, std::string id, int val)
{
    SYMBOL_INIT(type, id, val, int, 0, NULL);
}

Symbol::Symbol(Type* type, std::string id, bool val)
{
    SYMBOL_INIT(type, id, val, bool, 0, NULL);
}

Symbol::Symbol(Type* type, std::string id, SymbolTable* val)
{
    SYMBOL_INIT(type, id, val, class, 0, NULL);
}

Symbol::Symbol(Type* type, std::string id, SymbolTable* val, MethodDecl* f)
{
    SYMBOL_INIT(type, id, val, args, 0, f);
}

Symbol::Symbol(Type* type, std::string id)
{
    SYMBOL_INIT(type, id, NULL, class, 0, NULL);
}

SymbolTable::SymbolTable()
{
}

SymbolTable::SymbolTable(Type* type, ClassDecl* decl)
{
    this->parseVars(decl->vars);
    this->parseMethods(decl->decls);
    table["this"] = new Symbol(type, "this", this);
}

SymbolTable::SymbolTable(std::list<VarDecl*>* decl)
{
    this->parseVars(decl);
}

SymbolTable::SymbolTable(SymbolTable* st)
{
    this->table = st->table;
}

void SymbolTable::checkIfAlreadyDeclared(std::string id)
{
    if (!(table.find(id) == table.end()))
        std::cout << "WARNING: Redeclaration of variable " << id << std::endl;
}

void SymbolTable::parseVars(std::list<VarDecl*>* vars)
{
    std::list<VarDecl*>::iterator var_it;

    std::string id;
    Type* type;
    Symbol* symbol;

    for (var_it = vars->begin(); var_it != vars->end(); ++var_it)
    {
        id = (*var_it)->id->token;
        type = (*var_it)->type;
        this->checkIfAlreadyDeclared(id);
        symbol = new Symbol(type, id);
        table[id] = symbol;
        std::cout << "Symbol:" << symbol->id << std::endl;
    }
}

void SymbolTable::parseMethods(std::list<MethodDecl*>* decls)
{
    std::list<MethodDecl*>::iterator method_it;

    std::string id;
    Type* type;
    Symbol* symbol;

    for (method_it = decls->begin(); method_it != decls->end(); ++method_it)
    {
        id = (*method_it)->id->token;
        type = (*method_it)->type;
        this->checkIfAlreadyDeclared(id);
        SymbolTable* args = new SymbolTable((*method_it)->formals);
        symbol = new Symbol(type, id, args, *method_it);
        table[id] = symbol;
        std::cout << "Symbol:" << symbol->id << std::endl;
    }
}
