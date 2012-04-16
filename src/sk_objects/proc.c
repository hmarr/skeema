#include <stdlib.h>
#include <stdio.h>

#include "sk_objects/proc.h"
#include "sk_objects/string.h"

sk_Object *sk_proc_new(const char *name,
                       sk_Object *(*fn)(sk_Object *, sk_Object *),
                       int min_arity, int max_arity)
{
    sk_new_object_init(sk_Proc);
    obj->type = &sk_ProcType;
    obj->name = name;
    obj->fn = fn;
    obj->min_arity = min_arity;
    obj->max_arity = max_arity;
    return (sk_Object *)obj;
}

sk_Object *sk_proc_to_string(sk_Object *obj)
{
    int buf_len = strlen(sk_proc_name_cstr(obj)) + 4;  // # + name + ( + ) + \0
    char *buf = (char *)malloc(buf_len);
    snprintf(buf, buf_len, "#%s()", sk_proc_name_cstr(obj));
    sk_Object *str_obj = sk_string_new(buf);
    free(buf);
    return str_obj;
}

sk_ObjectType sk_ProcType = {
    "proc",             // name
    NULL,               // custom dealloc
    &sk_proc_to_string, // to_string
};

