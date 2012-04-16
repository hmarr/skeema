#ifndef _DICT_H
#define _DICT_H

#include "object.h"

typedef struct {
    sk_Object *entries;
} sk_Dict;

sk_Dict *sk_dict_new();
sk_Object *sk_dict_get(sk_Dict *dict, const char *key);
void sk_dict_set(sk_Dict *dict, const char *key, sk_Object *val);

#endif
