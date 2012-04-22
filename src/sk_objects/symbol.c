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
    sk_Object *str_obj;
    if (obj == sk_nil) {
        str_obj = sk_string_new("nil");
    } else {
        int buf_len = sk_symbol_length(obj) + 2;  // strlen + \0 + quote
        char *buf = (char *)malloc(buf_len);
        snprintf(buf, buf_len, "'%s", sk_symbol_cstr(obj));
        str_obj = sk_string_new(buf);
        free(buf);
    }
    return str_obj;
}

bool sk_symbol_is(sk_VM *vm, sk_Object *sym, const char *name)
{
    if (!sk_object_is(sym, sk_SymbolType)) {
        return false;
    }

    sk_Object *named_sym = sk_vm_get_symbol(vm, name);
    bool equal = sym == named_sym;
    sk_dec_ref(named_sym);

    return equal;
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

