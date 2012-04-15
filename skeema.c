#include <stdio.h>
#include <stdlib.h>

#include "object.h"
#include "scanner.h"
#include "parser.h"

void start_repl()
{
    char *line = (char *)malloc(1024);
    while (printf("skeema> "), fgets(line, 1024, stdin)) {
        if (line[0] == '\n') break;
        Object *sexp = parse(&line);
        if (sexp->type == CELL) {
            print_list(sexp);
        } else {
            print_object(sexp);
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc == 1) {
        //start_repl();
        char *code = "()";
        Object *sexp = parse(&code);
        if (sexp->type == CELL) {
            print_list(sexp);
        } else {
            print_object(sexp);
        }
        dec_ref(sexp);
    } else {
        Object *sexp = parse(&argv[1]);
        if (sexp->type == CELL) {
            print_list(sexp);
        } else {
            print_object(sexp);
        }
        dec_ref(sexp);
    }

    return 0;
}

