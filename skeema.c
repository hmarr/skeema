#include <stdio.h>
#include <stdlib.h>

#include "object.h"

int main(int argc, char *argv[])
{
    Object *harry = symbol_obj("harry");
    inspect_object(harry);
    dec_ref(harry);

    Object *a, *b, *c, *d, *e, *f;

    f = cons(float_obj(7.5), NULL);
    e = cons(symbol_obj("five"), f);
    d = cons(int_obj(4), e);
    c = cons(int_obj(3), d);
    b = cons(int_obj(2), c);
    a = cons(int_obj(1), b);

    dec_ref(f);
    dec_ref(e);
    dec_ref(d);
    dec_ref(c);
    dec_ref(b);

    deep_inspect_object(a);

    print_list(a);

    dec_ref(a);

    return 0;
}

