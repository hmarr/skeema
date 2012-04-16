#ifndef _INT_H
#define _INT_H

#include "sk_objects/object.h"

typedef struct {
    sk_ObjectHeader;
    long int_val;
} sk_Int;

sk_ObjectType sk_IntType;

// Return the long int value of a sk_Int.
#define sk_int_val(obj) (((sk_Int *)(obj))->int_val)

// Create a new sk_Int object from a long int.
sk_Object *sk_int_new(long value);

#endif
