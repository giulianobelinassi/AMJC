#ifndef TYPES_H
#define TYPES_H

#include <unordered_map>

struct Agnode_s;
struct Agraph_s;

class ClassDecl;

class Type
{
    public:
    static std::unordered_map<std::string, Type*> registered_types;
    static Type* getTypeFromStr(std::string);
    static Type* getDeclaredType(std::string);
    static Type* declareType(std::string, ClassDecl* decl=NULL);
    static void printDeclaredTypes(void);

    std::string id;
    ClassDecl* class_def;

    bool isBool();
    bool isInt();
    bool isArr();
    bool isClass();

    struct Agnode_s* buildGVNode(struct Agraph_s*);

    Type(std::string, ClassDecl*);
};

#endif
