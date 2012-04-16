#include <stdlib.h>
#include <stdio.h>

#include "sk_objects/string.h"

sk_Object *sk_string_new(const char *value)
{
    sk_new_object_init(sk_String);
    obj->type = &sk_StringType;
    obj->cstr = (char *)malloc(strlen(value) + 1);
    strcpy(obj->cstr, value);
    return (sk_Object *)obj;
}

sk_Object *sk_string_to_string(sk_Object *obj)
{
    int buf_len = sk_string_length(obj) + 3;  // strlen + \0 + 2 quotes
    char *buf = (char *)malloc(buf_len);
    snprintf(buf, buf_len, "\"%s\"", sk_string_cstr(obj));
    sk_Object *str_obj = sk_string_new(buf);
    free(buf);
    return str_obj;
}

void sk_string_dealloc(sk_Object *string)
{
    free(sk_string_cstr(string));
}

sk_ObjectType sk_StringType = {
    "string",             // name
    &sk_string_dealloc,   // custom dealloc
    &sk_string_to_string, // to_string
};

