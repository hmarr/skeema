#include <stdlib.h>
#include <stdio.h>

#include "sk_objects/symbol.h"
#include "sk_objects/string.h"

sk_Object *sk_symbol_new(const char *name)
{
    sk_new_object_init(sk_Symbol);
    obj->type = &sk_SymbolType;
    obj->name_cstr = (char *)malloc(strlen(name) + 1);
    strcpy(obj->name_cstr, name);
    return (sk_Object *)obj;
}

sk_Object *sk_symbol_to_string(sk_Object *obj)
{
    int buf_len = sk_symbol_length(obj) + 2;  // strlen + \0 + quote
    char *buf = (char *)malloc(buf_len);
    snprintf(buf, buf_len, "'%s", sk_symbol_cstr(obj));
    sk_Object *str_obj = sk_string_new(buf);
    free(buf);
    return str_obj;
}

void sk_symbol_dealloc(sk_Object *symbol)
{
    free(sk_symbol_cstr(symbol));
}

sk_ObjectType sk_SymbolType = {
    "symbol",             // name
    &sk_symbol_dealloc,   // custom dealloc
    &sk_symbol_to_string, // to_string
};

