#include "symboltable.h"
#include "types.h"
#include "declaration.h"
#include "expression.h"

#include <iostream>

#define SYMBOL_INIT(t, v, ty, off, f) do { \
    this->type = (t); \
    this->offset = (off); \
    this->val.as_##ty = (v); \
    this->func_body = (f); \
} while(0)

Symbol::Symbol(Type* type, int val)
{
    SYMBOL_INIT(type, val, int, 0, NULL);
}

Symbol::Symbol(Type* type, bool val)
{
    SYMBOL_INIT(type, val, bool, 0, NULL);
}

Symbol::Symbol(Type* type, SymbolTable* val)
{
    SYMBOL_INIT(type, val, class, 0, NULL);
}

Symbol::Symbol(Type* type, SymbolTable* val, MethodDecl* f)
{
    SYMBOL_INIT(type, val, args, 0, f);
}

Symbol::Symbol(Type* type)
{
    SYMBOL_INIT(type, NULL, class, 0, NULL);
}

SymbolTable::SymbolTable()
{
}

SymbolTable::SymbolTable(Type* type, ClassDecl* decl)
{
    this->parseVars(decl->vars);
    this->parseMethods(decl->decls);
    table["this"] = new Symbol(type, this);
}

SymbolTable::SymbolTable(std::list<VarDecl*>* decl)
{
    this->parseVars(decl);
}

SymbolTable::SymbolTable(SymbolTable* st)
{
    this->table = st->table;
}

bool SymbolTable::checkIfDeclared(std::string id)
{
    if (table.find(id) == table.end())
        return false;
    return true;
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

        if (checkIfDeclared(id))
            std::cout << "WARNING: Redeclaration of variable " << id << std::endl;

        symbol = new Symbol(type);
        table[id] = symbol;
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

        if (checkIfDeclared(id))
            std::cout << "WARNING: Redeclaration of variable " << id << std::endl;

        SymbolTable* args = new SymbolTable((*method_it)->formals);
        symbol = new Symbol(type, args, *method_it);
        table[id] = symbol;
    }
}

void SymbolTable::printTable()
{
    std::cout << "--Table Contents--" << std::endl;
    for (auto it : table)
    {
        Type* type = it.second->type;
        std::cout << " " << it.first << ":";
        if (type->isBool())
            std::cout << it.second->val.as_bool << std::endl;
        else if (type->isInt())
            std::cout << it.second->val.as_int << std::endl;
        else
            std::cout << it.second->val.as_class << std::endl;
    }
    std::cout << "------------------" << std::endl;
}

void print_table(SymbolTable* st)
{
    st->printTable();
}
