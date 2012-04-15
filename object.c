#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "object.h"


// Object constructors

Object *base_obj()
{
    Object *obj = (Object *)malloc(sizeof(Object));
    obj->ref_count = 1;
    return obj;
}

Object *int_obj(long value) {
    Object *obj = base_obj();
    obj->type = INTEGER;
    obj->int_val = value;
    return obj;
}

Object *float_obj(double value) {
    Object *obj = base_obj();
    obj->type = FLOAT;
    obj->float_val = value;
    return obj;
}

Object *symbol_obj(const char *value) {
    Object *obj = base_obj();
    obj->type = SYMBOL;
    obj->symbol_val = (char *)malloc(strlen(value) + 1);
    strcpy(obj->symbol_val, value);
    return obj;
}


// Memory management

void inc_ref(Object *obj)
{
    if (obj == NULL) return;
    obj->ref_count++;
}

void dec_ref(Object *obj)
{
    if (obj == NULL) return;
    if (obj->ref_count == 1) {
        debug("releasing object: ");
        inspect_object(obj);

        switch (obj->type) {
        case CELL:
            dec_ref(car(obj));
            dec_ref(cdr(obj));
            break;
        case SYMBOL:
            free(obj->symbol_val);
            break;
        default:
            break;
        }

        free(obj);
    } else {
        obj->ref_count--;
    }
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
    inc_ref(car);
    inc_ref(cdr);

    Object *obj = base_obj();
    obj->type = CELL;
    obj->cell_val = (Cell){ car, cdr };

    return obj;
}


// Utilities

int print_depth = 0;

void print_indent()
{
    int i;
    for (i = 0; i < print_depth * 2; i++, putchar(' ')) ;
}

void inspect_object(Object *obj)
{
    printf("<");
    if (obj == NULL) {
        printf("NULL>\n");
    } else {
        switch (obj->type) {
        case CELL:
            printf("Cell");
            break;
        case INTEGER:
            printf("Integer: %ld", obj->int_val);
            break;
        case FLOAT:
            printf("Float: %f", obj->float_val);
            break;
        case SYMBOL:
            printf("Symbol: %s", obj->symbol_val);
            break;
        default:
            printf("Unrecognised Object");
            break;
        }
        printf(", rc: %d>\n", obj->ref_count);
    }
}

void deep_inspect_object(Object *obj)
{
    inc_ref(obj);
    inspect_object(obj);

    if (obj != NULL && obj->type == CELL) {
        print_depth++;

        print_indent();
        printf("(car) ");
        deep_inspect_object(car(obj));

        print_indent();
        printf("(cdr) ");
        deep_inspect_object(cdr(obj));

        print_depth--;
    }
    dec_ref(obj);
}

void print_object(Object *obj)
{
    if (obj == NULL) {
        printf("NULL");
    } else {
        switch (obj->type) {
        case CELL:
            printf("Cell");
            break;
        case INTEGER:
            printf("%ld", obj->int_val);
            break;
        case FLOAT:
            printf("%.6f", obj->float_val);
            break;
        case SYMBOL:
            printf("\"%s\"", obj->symbol_val);
            break;
        default:
            printf("(Unrecognised Object)");
            break;
        }
    }
}

void print_list(Object *obj)
{
    if (obj->type != CELL) {
        debug("Invalid object type passed to print_list");
        return;
    }

    printf("[");
    int i = 0;
    while (obj != NULL && obj->type == CELL) {
        if (i++ != 0) {
           printf(", ");
        }
        print_object(car(obj));
        obj = cdr(obj);
    }
    printf("]\n");
}

