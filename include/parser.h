#ifndef _PARSER_H
#define _PARSER_H

#include "sk_objects/object.h"
#include "vm.h"

sk_Object *sk_parse(const sk_VM *vm, char **stream);

#endif

