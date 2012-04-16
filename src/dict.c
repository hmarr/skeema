#include <stdlib.h>
#include <string.h>

#include "dict.h"
#include "object.h"

sk_Dict *sk_dict_new()
{
    sk_Dict *dict = (sk_Dict *)malloc(sizeof(sk_Dict));
    dict->entries = NULL;
    return dict;
}

sk_Object *sk_dict_lookup_item(sk_Dict *dict, const char *key)
{
    sk_Object *item, *entry = dict->entries;
    while (entry != NULL) {
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
    return sk_cell_cdr(sk_dict_lookup_item(dict, key));
}

void sk_dict_set(sk_Dict *dict, const char *key, sk_Object *val)
{
    sk_Object *item = sk_dict_lookup_item(dict, key);
    if (item == NULL) {
        sk_Object *key_sym = sk_string_new(key);
        item = sk_cell_new(key_sym, val);
        sk_dec_ref(key_sym);

        // Create the entry
        dict->entries = sk_cell_new(item, dict->entries);
        sk_dec_ref(item);
    } else {
        // Overwrite the entry
        sk_cell_set_cdr(item, val);
    }
}

