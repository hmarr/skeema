#ifndef _DICT_H
#define _DICT_H

#include "sk_objects/object.h"

typedef struct {
    sk_ObjectHeader;
    sk_Object *head;
} sk_Dict;

sk_ObjectType sk_DictType;

sk_Object *sk_dict_new();

sk_Object *sk_dict_get(sk_Object *dict, const char *key);
void sk_dict_set(sk_Object *dict, const char *key, sk_Object *val);

void sk_dict_print(sk_Object *dict);

#endif
