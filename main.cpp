#include "tree.h"
#include "minijava.tab.h"

#include <stdio.h>
#include <string.h>

#define MAIN_COMPILE    0
#define MAIN_INTERP     1
#define MAIN_DUMP_TREE  2
#define MAIN_ARGERR     3

extern "C"{

extern FILE* yyin;

int parse_args(int argc, char* argv[])
{
    int i;

    for (i = 0; i < argc; ++i)
    {
        if (!strcmp(argv[i], "--compile"))
            return MAIN_COMPILE;
        else if (!strcmp(argv[i], "--interp"))
            return MAIN_INTERP;
        else if (!strcmp(argv[i], "--dump-tree"))
            return MAIN_DUMP_TREE;
    }
    return MAIN_ARGERR;
}

void print_usage_message()
{
    const char* message =
        "Minijava Interpreter and Compiler core\n"
        "This program should not be called alone. Use the minijc driver\n"
        "\n"
        "This program is Free Software and is distributed in the GNU Public License\n"
        "version 2. There is no warranty; not even from MERCHANTABILITY or FITNESS FOR\n"
        "A PARTICULAR PURPOSE\n";

    fputs(message, stderr);
}

int main(int argc, char* argv[])
{
    Program* program;
    int arg;
    yyin = stdin;

    arg = parse_args(argc, argv);

    switch (arg)
    {
        case MAIN_COMPILE:
            yyparse(&program);
            program->compile();
        break;

        case MAIN_INTERP:
            yyparse(&program);
            program->interp();
        break;

        case MAIN_DUMP_TREE:
            yyparse(&program);
            program->generateGraphViz();
        break;

        default:
            print_usage_message();
        break;
    }

    return 0;
}
}
