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

#endif
