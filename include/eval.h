#ifndef _EVAL_H
#define _EVAL_H

#include "sk_objects/object.h"
#include "vm.h"

sk_Object *sk_eval(sk_VM *vm, sk_Object *exp);

#endif
