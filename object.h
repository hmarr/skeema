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
    union {
        Cell cell_val;
        long int_val;
        double float_val;
        const char *symbol_val;
    };
};

// Object constructors
Object *int_obj(long value);
Object *float_obj(double value);

// List operations
Object *car(Object *obj);
Object *cdr(Object *obj);
Object *cons(Object *car, Object *cdr);

// Utilities
void print_object(Object *obj);

#endif
