#ifndef _SYMBOL_H
#define _SYMBOL_H

#include "sk_objects/object.h"

typedef struct {
    sk_ObjectHeader;
    char *name_cstr;
} sk_Symbol;

sk_ObjectType sk_SymbolType;

// Similar to the equivalent sk_String functions above.
#define sk_symbol_cstr(obj) (((sk_Symbol *)(obj))->name_cstr)
#define sk_symbol_length(obj) (strlen(sk_symbol_cstr(obj)))
#define sk_symbol_name_eq(obj, str) (strcmp(sk_symbol_cstr(obj), str) == 0)

// Create a new sk_Symbol object from a C string. Generally this should not be
// used directly - prefer sk_vm_get_symbol, which uses the VM's symbol table.
sk_Object *sk_symbol_new(const char *name);

void sk_symbol_dealloc(sk_Object *symbol);

#endif
