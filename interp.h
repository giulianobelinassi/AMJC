#ifdef INTERP_H
#define INTERP_H

class SymbolTable;

enum interp {
    INTERP_INT,
    INTERP_BOOL,
    INTERP_TBL,
    INTERP_ARR
};

struct interp_ret
{
    union {
        int as_int;
        bool as_bool;
        int* as_arr;
        SymbolTable* as_tbl;
    } val;
    enum interp is;
};

#endif
