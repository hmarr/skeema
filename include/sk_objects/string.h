#ifndef _STRING_H
#define _STRING_H

#include "sk_objects/object.h"

typedef struct {
    sk_ObjectHeader;
    char *cstr;
} sk_String;

sk_ObjectType sk_StringType;

// Return the null-terminated string value of a sk_String (without a reference).
#define sk_string_cstr(obj) (((sk_String *)(obj))->cstr)
// Return the length of a sk_String.
#define sk_string_length(obj) (strlen(sk_string_cstr(obj)))
// Check if a string is equal to a given C string.
#define sk_string_eq_cstr(obj, cstr) (strcmp(sk_string_cstr(obj), cstr) == 0)

// Create a new sk_String object from a null-terminated string. The contents
// of the string are copied to a new buffer on creation, so no reference to the
// cstring is held.
sk_Object *sk_string_new(const char *value);

void sk_string_dealloc(sk_Object *symbol);

#endif
