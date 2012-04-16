#ifndef _OBJECT_H
#define _OBJECT_H

#include <stdbool.h>
#include <string.h>


// Forward declaration for sk_ObjectType
typedef struct sk_Object sk_Object;

// Object type definitions. Every sk_Object has a 'type' field that corresponds
// to one of these.
typedef struct {
    const char *name;
    void (*dealloc)(sk_Object *);
    sk_Object *(*to_string)(sk_Object *);
} sk_ObjectType;

// Object header to be included in each object struct.
#define sk_ObjectHeader  \
    sk_ObjectType *type; \
    int ref_count

// This is the base type that objects are passed around as. Generally sk_Object
// isn't used, one of the concrete types (sk_Cell, sk_Int, etc) are used, so
// cast to them to inside funcions.
//
// Each object type has a constructor that takes the form sk_<type>_new. The
// refcount of the returned object will be 1, so it'll need to be dec_ref'd at
// some point.
struct sk_Object {
    sk_ObjectHeader;
};

#define sk_object_is(obj, T)  (obj->type == &T)

// Constructor helper
#define sk_new_object_init(T)         \
    T *obj;                           \
    do {                              \
        obj = (T *)malloc(sizeof(T)); \
        obj->ref_count = 1;           \
    } while (0)


// nil is a singleton symbol - used instead of NULL for signalling the end of
// lists, and also represents the empty list.
sk_Object *sk_nil_sym();
#define sk_nil (sk_nil_sym())


// Increment the reference count of an object.
sk_Object *sk_inc_ref(sk_Object *obj);

// Decrement the reference count of an object. If the reference count hits 0,
// the object will be deallocated.
sk_Object *sk_dec_ref(sk_Object *obj);


// Return a string (sk_String) representation of the object passed in. The
// string is returned with a reference so it'll need to be dec_ref'd.
sk_Object *sk_object_to_string(sk_Object *obj);

// Print the object to stdout.
void sk_object_print(sk_Object *obj);

/*

// Cell



// Int

typedef struct {
    sk_Object_HEADER
    long int_val;
} sk_Int;

// Return the long int value of a sk_Int.
#define sk_int_val(obj) (((sk_Int *)(obj))->int_val)

// Create a new sk_Int object from a long int.
sk_Object *sk_int_new(long value);


// Float

typedef struct {
    sk_Object_HEADER
    double float_val;
} sk_Float;

// Return the double value of a sk_Float.
#define sk_float_val(obj) (((sk_Float *)(obj))->float_val)

// Create a new sk_Float object from a double.
sk_Object *sk_float_new(double value);


// String

typedef struct {
    sk_Object_HEADER
    char *cstr;
} sk_String;

// Return the null-terminated string value of a sk_String (without a reference).
#define sk_string_cstr(obj) (((sk_String *)(obj))->cstr)
// Return the length of a sk_String.
#define sk_string_length(obj) (strlen(sk_string_cstr(obj)))

// Create a new sk_String object from a null-terminated string. The contents
// of the string are copied to a new buffer on creation, so no reference to the
// cstring is held.
sk_Object *sk_string_new(const char *value);


// Symbol

typedef struct {
    sk_Object_HEADER
    char *name_cstr;
} sk_Symbol;

// Similar to the equivalent sk_String functions above.
#define sk_symbol_cstr(obj) (((sk_Symbol *)(obj))->name_cstr)
#define sk_symbol_length(obj) (strlen(sk_symbol_cstr(obj)))
#define sk_symbol_name_eq(obj, str) (strcmp(sk_symbol_cstr(obj), str))

// Create a new sk_Symbol object from a C string. Generally this should not be
// used directly - prefer sk_vm_get_symbol, which uses the VM's symbol table.
sk_Object *sk_symbol_new(const char *name);

// Print an object to stdout without dicking around with sk_Objects. This is
// useful when debugging sk_dec_ref as it won't result in recursive calls.
void sk_object_debug_print(sk_Object *obj);
*/

#endif