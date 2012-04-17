#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "skeema.h"

void start_repl(sk_VM *vm)
{
    char *line = (char *)malloc(1024);
    while (printf("skeema> "), fgets(line, 1024, stdin)) {
        if (line[0] == '\n') continue;
        sk_Object *exp = sk_parse(vm, &line);
        sk_Object *result = sk_eval(vm, exp);
        sk_dec_ref(exp);

        fputs(" => ", stdout);
        sk_object_print(result);
        sk_dec_ref(result);
    }
}

int main(int argc, char *argv[])
{
    sk_VM *vm = sk_vm_new();

    if (argc == 1) {
        start_repl(vm);
    } else {
        sk_Object *exp = sk_parse(vm, &argv[1]);
        sk_Object *result = sk_eval(vm, exp);
        sk_dec_ref(exp);

        fputs(" => ", stdout);
        sk_object_print(result);
        sk_dec_ref(result);
    }

    sk_vm_dealloc(vm);

    return 0;
}

