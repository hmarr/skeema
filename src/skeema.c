#include <stdio.h>
#include <stdlib.h>

#include "object.h"
#include "scanner.h"
#include "parser.h"

void start_repl()
{
    char *line = (char *)malloc(1024);
    while (printf("skeema> "), fgets(line, 1024, stdin)) {
        if (line[0] == '\n') continue;
        sk_Object *sexp = sk_parse(&line);
        sk_object_print(sexp);
        sk_dec_ref(sexp);
    }
}

int main(int argc, char *argv[])
{
    if (argc == 1) {
        start_repl();
    } else {
        sk_Object *sexp = sk_parse(&argv[1]);
        sk_object_print(sexp);
        sk_dec_ref(sexp);
    }

    /*char *code = "(3 4.5 (upcase \"harry\"))";
    sk_Object *sexp = sk_parse(&code);
    puts(sk_string_cstr(sk_object_to_string(sexp)));
    sk_dec_ref(sexp);

    sk_Object *a, *b, *c, *d, *e, *f;

    f = sk_cell_new(sk_float_new(7.5), NULL);
    e = sk_cell_new(sk_symbol_new("some-symbol"), f);
    d = sk_cell_new(sk_string_new("a string"), e);
    c = sk_cell_new(sk_int_new(3), d);
    b = sk_cell_new(sk_int_new(2), c);
    a = sk_cell_new(sk_int_new(1), b);

    puts(sk_string_cstr(sk_object_to_string(a)));

    sk_dec_ref(f);
    sk_dec_ref(e);
    sk_dec_ref(d);
    sk_dec_ref(c);
    sk_dec_ref(b);*/

    return 0;
}

