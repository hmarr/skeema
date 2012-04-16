#include <stdlib.h>
#include <stdio.h>

#include "sk_objects/float.h"
#include "sk_objects/string.h"

sk_Object *sk_float_new(double value)
{
    sk_new_object_init(sk_Float);
    obj->type = &sk_FloatType;
    obj->float_val = value;
    return (sk_Object *)obj;
}

sk_Object *sk_float_to_string(sk_Object *obj)
{
    char *buf = (char *)malloc(28);  // max formatted double = 28 bytes
    snprintf(buf, 28, "%.20g", sk_float_val(obj));
    sk_Object *str_obj = sk_string_new(buf);
    free(buf);
    return str_obj;
}

sk_ObjectType sk_FloatType = {
    "float",             // name
    NULL,                // custom dealloc
    &sk_float_to_string, // to_string
};

