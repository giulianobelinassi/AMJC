

#define COPY_STR(dest, src)  do {\
    size_t len = strlen(src); \
    dest = new char[len]; \
    strncpy((char*) dest, src, len); } \
while(0)

#define ZERO_CHILD_VERTEX(v, name) do { \
    v = agnode(g, NULL, 1); \
    agset(v, (char*) "label", (char*) name); \
} while(0)

#define ONE_CHILD_VERTEX(v, name, c1) do { \
    v = agnode(g, NULL, 1); \
    agset(v, (char*) "label", (char*) name); \
    Agnode_t* _c1 = c1? c1->buildGVNode(g): agnode(g, (char*) "NULL", 1); \
    agedge(g, v, _c1, 0, 1); \
} while(0)

#define TWO_CHILD_VERTEX(v, name, c1, c2) do { \
    v = agnode(g, NULL, 1); \
    agset(v, (char*) "label", (char*) name); \
    Agnode_t* _c1 = c1? c1->buildGVNode(g): agnode(g, (char*) "NULL", 1); \
    Agnode_t* _c2 = c2? c2->buildGVNode(g): agnode(g, (char*) "NULL", 1); \
    agedge(g, v, _c1, 0, 1); \
    agedge(g, v, _c2, 0, 1); \
} while(0)

#define THREE_CHILD_VERTEX(v, name, c1, c2, c3) do { \
    v = agnode(g, NULL, 1); \
    agset(v, (char*) "label", (char*) name); \
    Agnode_t* _c1 = c1? c1->buildGVNode(g): agnode(g, (char*) "NULL", 1);\
    Agnode_t* _c2 = c2? c2->buildGVNode(g): agnode(g, (char*) "NULL", 1);\
    Agnode_t* _c3 = c3? c3->buildGVNode(g): agnode(g, (char*) "NULL", 1);\
    agedge(g, v, _c1, 0, 1); \
    agedge(g, v, _c2, 0, 1); \
    agedge(g, v, _c3, 0, 1); \
} while(0)

#define FIVE_CHILD_VERTEX(v, name, c1, c2, c3, c4, c5) do { \
    v = agnode(g, NULL, 1); \
    agset(v, (char*) "label", (char*) name); \
    Agnode_t* _c1 = c1? c1->buildGVNode(g): agnode(g, (char*) "NULL", 1);\
    Agnode_t* _c2 = c2? c2->buildGVNode(g): agnode(g, (char*) "NULL", 1);\
    Agnode_t* _c3 = c3? c3->buildGVNode(g): agnode(g, (char*) "NULL", 1);\
    Agnode_t* _c4 = c4? c4->buildGVNode(g): agnode(g, (char*) "NULL", 1);\
    Agnode_t* _c5 = c5? c5->buildGVNode(g): agnode(g, (char*) "NULL", 1);\
    agedge(g, v, _c1, 0, 1); \
    agedge(g, v, _c2, 0, 1); \
    agedge(g, v, _c3, 0, 1); \
    agedge(g, v, _c4, 0, 1); \
    agedge(g, v, _c5, 0, 1); \
} while(0)

