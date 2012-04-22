#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "sk_objects/object.h"
#include "sk_objects/cell.h"
#include "sk_objects/symbol.h"
#include "sk_objects/string.h"
#include "sk_objects/int.h"


sk_Object *sk_nil_sym_val = NULL;
sk_Object *sk_nil_sym()
{
    if (sk_nil_sym_val == NULL) {
        sk_nil_sym_val = sk_symbol_new("nil");
    }
    return sk_nil_sym_val;
}


// Memory management

sk_Object *sk_inc_ref(sk_Object *obj)
{
    if (obj == sk_nil || obj == NULL) return obj;
    obj->ref_count++;
    return obj;
}

sk_Object *sk_dec_ref(sk_Object *obj)
{
    if (obj == sk_nil || obj == NULL) return obj;
    if (obj->ref_count == 1) {
        //printf("releasing object: %s\n", obj->type->name);

        // allow objects like cells to clean themselves up
        if (obj->type->dealloc != NULL) {
            (obj->type->dealloc)(obj);
        }
        free(obj);

        return NULL;
    } else {
        //printf("decreffing object: %s\n", obj->type->name);
        obj->ref_count--;
        return obj;
    }
}


// Utilities

sk_Object *sk_object_to_string(sk_Object *obj)
{
    if (obj == NULL) {
        return sk_string_new("NULL");
    } else if (obj->type->to_string == NULL) {
        return sk_string_new(obj->type->name);
    }
    return (obj->type->to_string)(obj);
}

void sk_object_print(sk_Object *obj)
{
    sk_Object *obj_str = sk_object_to_string(obj);
    puts(sk_string_cstr(obj_str));
    sk_dec_ref(obj_str);
}

