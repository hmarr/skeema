#ifndef _VM_H
#define _VM_H

#include "sk_objects/object.h"

typedef struct {
    sk_Object *symbol_table;
    sk_Object *scope;
} sk_VM;

sk_VM *sk_vm_new();
void sk_vm_dealloc(sk_VM *vm);
sk_Object *sk_vm_get_symbol(const sk_VM *vm, const char *name);

#endif
