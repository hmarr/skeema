#include <stdio.h>
#include <stdlib.h>

#include "object.h"
#include "scanner.h"

int main(int argc, char *argv[])
{
    /*Object *a, *b, *c, *d, *e, *f;

    f = cons(float_obj(7.5), NULL);
    e = cons(symbol_obj("some-symbol"), f);
    d = cons(string_obj("a string"), e);
    c = cons(int_obj(3), d);
    b = cons(int_obj(2), c);
    a = cons(int_obj(1), b);

    dec_ref(f);
    dec_ref(e);
    dec_ref(d);
    dec_ref(c);
    dec_ref(b);

    print_list(a);
    print_object(a);

    dec_ref(a);

    Object *harry = string_obj("harry");
    print_object(harry);
    dec_ref(harry);
    */

    char *stream = "     (======== 5 69 ) (Math.pi 6 (mmmmmmm hiya))";
    Object *token;
    while ((token = read_token(&stream)) != NULL) {
        print_object(token);
    }

    return 0;
}

