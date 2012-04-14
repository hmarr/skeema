#include <stdio.h>
#include <stdlib.h>

#include "object.h"

int main(int argc, char *argv[])
{
    Object obj = { INTEGER, { .int_val = 5 } };
    print_object(&obj);

    Object *a, *b, *c, *d, *e, *f;

    f = cons(float_obj(7.5), NULL);
    e = cons(int_obj(5), f);
    d = cons(int_obj(4), e);
    c = cons(int_obj(3), d);
    b = cons(int_obj(2), c);
    a = cons(int_obj(1), b);

    print_object(a);

    return 0;
}

