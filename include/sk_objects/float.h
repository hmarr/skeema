#ifndef _FLOAT_H
#define _FLOAT_H

#include "sk_objects/object.h"

typedef struct {
    sk_ObjectHeader;
    double float_val;
} sk_Float;

sk_ObjectType sk_FloatType;

// Return the double value of a sk_Float.
#define sk_float_val(obj) (((sk_Float *)(obj))->float_val)

// Create a new sk_Float object from a double.
sk_Object *sk_float_new(double value);

#endif
