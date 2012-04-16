#include <stdlib.h>

#include "vm.h"
#include "object.h"

sk_VM *sk_vm_new()
{
    sk_VM *vm = (sk_VM *)malloc(sizeof(sk_VM));
    vm->symbol_table = sk_dict_new();
    return vm;
}

void sk_vm_dealloc(sk_VM *vm)
{
    sk_dict_dealloc(vm->symbol_table);
    free(vm);
}

sk_Object *sk_vm_get_symbol(const sk_VM *vm, const char *name)
{
    sk_Object *obj = sk_inc_ref(sk_dict_get(vm->symbol_table, name));
    if (obj == NULL) {
        obj = sk_symbol_new(name);
        sk_dict_set(vm->symbol_table, name, obj);
    }
    return obj;
}

