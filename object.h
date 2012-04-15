#ifndef _OBJECT_H
#define _OBJECT_H

typedef enum {
    CELL,
    INTEGER,
    FLOAT,
    SYMBOL,
    NIL
} ObjectType;

// Object forward declaration for Cell union
struct Object;
typedef struct Object Object;

typedef struct {
     Object *car;
     Object *cdr;
} Cell;

// Each object has a type (ObjectType) and a value in the union
struct Object {
    ObjectType type;
    unsigned int ref_count;

    union {
        Cell cell_val;
        long int_val;
        double float_val;
        char *symbol_val;
    };
};

// Object constructors
Object *int_obj(long value);
Object *float_obj(double value);
Object *symbol_obj(const char *value);

// Memory management
void inc_ref(Object *obj);
void dec_ref(Object *obj);

// List operations
Object *car(Object *obj);
Object *cdr(Object *obj);
Object *cons(Object *car, Object *cdr);

// Utilities
void inspect_object(Object *obj);
void deep_inspect_object(Object *obj);
void print_object(Object *obj);
void print_list(Object *obj);

#endif
