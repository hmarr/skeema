#include <stdlib.h>
#include <stdio.h>

#include "utils.h"
#include "sk_objects/cell.h"
#include "sk_objects/string.h"

sk_Object *sk_cell_new(sk_Object *car, sk_Object *cdr)
{
    sk_new_object_init(sk_Cell);
    obj->type = &sk_CellType;
    obj->car = obj->cdr = sk_nil;
    sk_cell_set_car((sk_Object *)obj, car);
    sk_cell_set_cdr((sk_Object *)obj, cdr);
    return (sk_Object *)obj;
}

void sk_cell_set_car(sk_Object *obj, sk_Object *car)
{
    sk_dec_ref(sk_cell_car(obj));
    ((sk_Cell *)obj)->car = sk_inc_ref(car);
}

void sk_cell_set_cdr(sk_Object *obj, sk_Object *cdr)
{
    sk_dec_ref(sk_cell_cdr(obj));
    ((sk_Cell *)obj)->cdr = sk_inc_ref(cdr);
}

int sk_cell_append(sk_Object *list, sk_Object *obj)
{
    if (!sk_object_is(list, sk_CellType)) {
        error_obj("called append with invalid object %s", list);
        return -1;
    }

    while (sk_object_is(sk_cell_cdr(list), sk_CellType)) {
        list = sk_cell_cdr(list);
    }

    if (sk_cell_cdr(list) != sk_nil) {
        error_obj("tried to append %s to improper list", obj);
        return -1;
    }

    sk_Object *new_cell = sk_cell_new(obj, sk_nil);
    sk_cell_set_cdr(list, new_cell);
    sk_dec_ref(new_cell);

    return 0;
}

sk_Object *sk_cell_to_string(sk_Object *obj)
{
    sk_Object *car_str = sk_object_to_string(sk_cell_car(obj)),
              *cdr_str = sk_object_to_string(sk_cell_cdr(obj));

    int buf_len = sk_string_length(car_str) + sk_string_length(cdr_str);
    buf_len += 4;  // 2 parens + space + \0

    char *buf = (char *)malloc(buf_len);
    snprintf(buf, buf_len, "(%s %s)", sk_string_cstr(car_str),
                                      sk_string_cstr(cdr_str));
    sk_dec_ref(car_str);
    sk_dec_ref(cdr_str);

    sk_Object *str_obj = sk_string_new(buf);
    free(buf);

    return str_obj;
}

void sk_cell_dealloc(sk_Object *cell)
{
    sk_dec_ref(sk_cell_car(cell));
    sk_dec_ref(sk_cell_cdr(cell));
}

sk_ObjectType sk_CellType = {
    "cell",             // name
    &sk_cell_dealloc,   // custom dealloc
    &sk_cell_to_string, // to_string
};

