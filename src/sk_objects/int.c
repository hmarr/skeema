#include <stdlib.h>
#include <stdio.h>

#include "sk_objects/int.h"
#include "sk_objects/string.h"

sk_Object *sk_int_new(long value)
{
    sk_new_object_init(sk_Int);
    obj->type = &sk_IntType;
    obj->int_val = value;
    return (sk_Object *)obj;
}

sk_Object *sk_int_to_string(sk_Object *obj)
{
    char *buf = (char *)malloc(21);  // max 64-bit int = 20 bytes + \0
    snprintf(buf, 21, "%ld", sk_int_val(obj));
    sk_Object *str_obj = sk_string_new(buf);
    free(buf);
    return str_obj;
}

sk_ObjectType sk_IntType = {
    "int",              // name
    NULL,               // custom dealloc
    &sk_int_to_string,  // to_string
};

