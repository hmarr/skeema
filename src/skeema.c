#include <stdio.h>
#include <stdlib.h>

#include "skeema.h"

/*
#include "object.h"
#include "scanner.h"
#include "parser.h"
#include "dict.h"
#include "vm.h"
*/

/*
void start_repl(sk_VM *vm)
{
    char *line = (char *)malloc(1024);
    while (printf("skeema> "), fgets(line, 1024, stdin)) {
        if (line[0] == '\n') continue;
        sk_Object *sexp = sk_parse(vm, &line);
        sk_object_print(sexp);
        sk_dec_ref(sexp);
    }
    //sk_dict_print(vm->symbol_table);
}
*/

int main(int argc, char *argv[])
{
    /*
    sk_VM *vm = sk_vm_new();

    if (argc == 1) {
        start_repl(vm);
    } else {
        sk_Object *sexp = sk_parse(vm, &argv[1]);
        sk_object_print(sexp);
        sk_dec_ref(sexp);
    }

    sk_vm_dealloc(vm);
    */

    sk_VM *vm = sk_vm_new();

    char *code = "(3 4.5 (upcase 8 6.5 \"harry\") 7)";
    sk_Object *exp = sk_parse(vm, &code);
    puts("sk_object_to_string(exp) {");
    sk_Object *exp_str = sk_object_to_string(exp);
    puts("}\n");

    puts("exp_str {");
    puts(sk_string_cstr(exp_str));
    puts("}\n");
    sk_dec_ref(exp);
    sk_dec_ref(exp_str);

    sk_vm_dealloc(vm);

    /*
    sk_Object *val, *a, *b, *c, *d;

    val = sk_dict_new();
    sk_Object *foo = sk_string_new("foo");
    sk_dict_set(val, "foo", foo);
    sk_dec_ref(foo);
    d = sk_cell_new(val, sk_nil);
    sk_dict_print(val);
    sk_dec_ref(val);

    val = sk_symbol_new("some-symbol");
    c = sk_cell_new(val, d);
    sk_dec_ref(val);

    val = sk_string_new("test string");
    b = sk_cell_new(val, c);
    sk_dec_ref(val);

    val = sk_int_new(12345);
    a = sk_cell_new(val, b);
    sk_dec_ref(val);

    sk_Object *list_str = sk_object_to_string(a);
    puts(sk_string_cstr(list_str));
    sk_dec_ref(list_str);

    sk_dec_ref(d);
    sk_dec_ref(c);
    sk_dec_ref(b);
    sk_dec_ref(a);
    */

    return 0;
}

