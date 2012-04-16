#ifndef _OBJECT_H
#define _OBJECT_H

#include <stdbool.h>


// Object

typedef enum {
    sk_CELL,
    sk_SYMBOL,
    sk_STRING,
    sk_INT,
    sk_FLOAT,
    sk_NIL
} sk_ObjectType;

#define sk_Object_HEADER  \
    sk_ObjectType type; \
    int ref_count;

typedef struct {
    sk_Object_HEADER
} sk_Object;

// Memory management
sk_Object *sk_inc_ref(sk_Object *obj);
sk_Object *sk_dec_ref(sk_Object *obj);

sk_Object *sk_object_to_string(sk_Object *obj);


// Cell

typedef struct {
    sk_Object_HEADER
    sk_Object *car;
    sk_Object *cdr;
} sk_Cell;

#define sk_cell_car(obj) (((sk_Cell *)(obj))->car)
#define sk_cell_cdr(obj) (((sk_Cell *)(obj))->cdr)

sk_Object *sk_cell_new(sk_Object *car, sk_Object *cdr);

void sk_cell_set_car(sk_Object *obj, sk_Object *car);
void sk_cell_set_cdr(sk_Object *obj, sk_Object *cdr);


// Int

typedef struct {
    sk_Object_HEADER
    long int_val;
} sk_Int;

#define sk_int_val(obj) (((sk_Int *)(obj))->int_val)

sk_Object *sk_int_new(long value);


// Float

typedef struct {
    sk_Object_HEADER
    double float_val;
} sk_Float;

#define sk_float_val(obj) (((sk_Float *)(obj))->float_val)

sk_Object *sk_float_new(double value);


// String

typedef struct {
    sk_Object_HEADER
    char *cstr;
} sk_String;

#define sk_string_cstr(obj) (((sk_String *)(obj))->cstr)
#define sk_string_length(obj) (strlen(sk_string_cstr(obj)))

sk_Object *sk_string_new(const char *value);


// Symbol

typedef struct {
    sk_Object_HEADER
    char *name_cstr;
} sk_Symbol;

#define sk_symbol_cstr(obj) (((sk_Symbol *)(obj))->name_cstr)
#define sk_symbol_length(obj) (strlen(sk_symbol_cstr(obj)))

sk_Object *sk_symbol_new(const char *name);


// List operations

// Utilities
//const char *object_type_str(Object *obj);
//void object_debug_print(Object *obj);
//void inspect_object(Object *obj);
//void deep_inspect_object(Object *obj);
//void print_object(Object *obj);
//void print_list(Object *obj);
//Object *object_str(Object *obj);
//Object *list_str(Object *obj);
void sk_object_debug_print(sk_Object *obj);

#endif
