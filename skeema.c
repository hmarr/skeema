#include <stdio.h>
#include <stdlib.h>

#include "object.h"
#include "scanner.h"
#include "parser.h"

int main(int argc, char *argv[])
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

    return 0;
}

