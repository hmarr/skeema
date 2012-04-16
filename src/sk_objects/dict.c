#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utils.h"
#include "sk_objects/dict.h"
#include "sk_objects/cell.h"
#include "sk_objects/string.h"

// Return the first element of the dict
#define sk_dict_head(obj) (((sk_Dict *)(obj))->head)

sk_Object *sk_dict_new()
{
    sk_new_object_init(sk_Dict);
    obj->type = &sk_DictType;
    obj->head = sk_nil;
    return (sk_Object *)obj;
}

sk_Object *sk_dict_lookup_item(sk_Dict *dict, const char *key)
{
    sk_Object *item, *entry = sk_dict_head(dict);
    while (entry != sk_nil) {
        item = sk_cell_car(entry);
        if (strcmp(sk_string_cstr(sk_cell_car(item)), key) == 0) {
            return item;
        }
        entry = sk_cell_cdr(entry);
    }

    return NULL;
}

sk_Object *sk_dict_get(sk_Object *dict, const char *key)
{
    sk_Object *item = sk_dict_lookup_item((sk_Dict *)dict, key);
    if (item == NULL) {
        return NULL;
    }
    return sk_cell_cdr(item);
}

void sk_dict_set(sk_Object *dict, const char *key, sk_Object *val)
{
    sk_Object *item = sk_dict_lookup_item((sk_Dict *)dict, key);
    if (item == NULL) {
        sk_Object *key_sym = sk_string_new(key);
        item = sk_cell_new(key_sym, val);
        sk_dec_ref(key_sym);

        // Create the entry
        sk_Object *old_head = sk_dict_head(dict);
        ((sk_Dict *)dict)->head = sk_cell_new(item, old_head);
        // old head and item now in the capable hands of new head
        sk_dec_ref(old_head);
        sk_dec_ref(item);
    } else {
        // Overwrite the entry
        sk_cell_set_cdr(item, val);
    }
}

void sk_dict_print(sk_Object *dict)
{
    sk_Object *item, *car_str, *cdr_str, *entry = sk_dict_head(dict);
    puts("{");
    while (entry != sk_nil) {
        item = sk_cell_car(entry);
        car_str = sk_object_to_string(sk_cell_car(item));
        cdr_str = sk_object_to_string(sk_cell_cdr(item));
        printf("  %s => %s,\n", sk_string_cstr(car_str),
                                sk_string_cstr(cdr_str));
        entry = sk_cell_cdr(entry);
    }
    puts("}");
}

void sk_dict_dealloc(sk_Object *dict)
{
    sk_dec_ref(sk_dict_head(dict));
}

sk_ObjectType sk_DictType = {
    "dict",             // name
    &sk_dict_dealloc,   // custom dealloc
    NULL, // to_string
};

