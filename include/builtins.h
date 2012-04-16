#ifndef _BUILTINS_H
#define _BUILTINS_H

#include "sk_objects/object.h"

sk_Object *sk_builtins_sum(sk_Object *scope, sk_Object *args);
void sk_builtins_populate_scope(sk_Object *scope);

#endif
