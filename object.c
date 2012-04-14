#include <stdio.h>
#include <stdlib.h>

#include "object.h"


// Object constructors

Object *int_obj(long value) {
    Object *obj = (Object *)malloc(sizeof(Object));
    obj->type = INTEGER;
    obj->int_val = value;
    return obj;
}

Object *float_obj(double value) {
    Object *obj = (Object *)malloc(sizeof(Object));
    obj->type = FLOAT;
    obj->float_val = value;
    return obj;
}


// List operations

Object *car(Object *obj)
{
    return obj->cell_val.car;
}

Object *cdr(Object *obj)
{
    return obj->cell_val.cdr;
}

Object *cons(Object *car, Object *cdr)
{
    Object *obj = (Object *)malloc(sizeof(Object));
    obj->type = CELL;
    obj->cell_val = (Cell){ car, cdr };

    return obj;
}


// Utilities

void print_object(Object *obj)
{
    switch (obj->type) {
    case CELL:
        printf("<Cell>\n");
        print_object(car(obj));
        if (cdr(obj) != NULL) {
            print_object(cdr(obj));
        }
        break;
    case INTEGER:
        printf("<Integer: %ld>\n", obj->int_val);
        break;
    case FLOAT:
        printf("<Float: %f>\n", obj->float_val);
        break;
    default:
        printf("<Unrecognised Object>\n");
        break;
    }
}

