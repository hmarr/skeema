#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "dict.h"
#include "object.h"

sk_Dict *sk_dict_new()
{
    sk_Dict *dict = (sk_Dict *)malloc(sizeof(sk_Dict));
    dict->entries = sk_nil;
    return dict;
}

void sk_dict_dealloc(sk_Dict *dict)
{
    sk_dec_ref(dict->entries);
    free(dict);
}

sk_Object *sk_dict_lookup_item(sk_Dict *dict, const char *key)
{
    sk_Object *item, *entry = dict->entries;
    while (entry != sk_nil) {
        item = sk_cell_car(entry);
        if (strcmp(sk_string_cstr(sk_cell_car(item)), key) == 0) {
            return item;
        }
        entry = sk_cell_cdr(entry);
    }

    return NULL;
}

sk_Object *sk_dict_get(sk_Dict *dict, const char *key)
{
    sk_Object *item = sk_dict_lookup_item(dict, key);
    if (item == NULL) {
        return NULL;
    }
    return sk_cell_cdr(item);
}

void sk_dict_set(sk_Dict *dict, const char *key, sk_Object *val)
{
    sk_Object *item = sk_dict_lookup_item(dict, key);
    if (item == NULL) {
        sk_Object *key_sym = sk_string_new(key);
        item = sk_cell_new(key_sym, val);
        sk_dec_ref(key_sym);

        // Create the entry
        sk_Object *old_head = dict->entries;
        dict->entries = sk_cell_new(item, dict->entries);
        // old head now in the capable hands of new head
        sk_dec_ref(old_head);
        sk_dec_ref(item);
    } else {
        // Overwrite the entry
        sk_cell_set_cdr(item, val);
    }
}

void sk_dict_print(sk_Dict *dict)
{
    sk_Object *item, *car_str, *cdr_str, *entry = dict->entries;
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

