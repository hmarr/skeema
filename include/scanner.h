#ifndef _SCANNER_H
#define _SCANNER_H

#include "object.h"
#include "vm.h"

sk_Object *sk_read_symbol(const sk_VM *vm, char **stream_ptr);
sk_Object *sk_read_string_literal(char **stream_ptr);

#endif
