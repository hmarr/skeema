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

Object *string_obj(const char *value) {
    Object *obj = base_obj();
    obj->type = STRING;
    obj->string_val = (char *)malloc(strlen(value) + 1);
    strcpy(obj->string_val, value);
    return obj;
}


// Memory management

Object *inc_ref(Object *obj)
{
    if (obj == NULL) return NULL;
    obj->ref_count++;
    return obj;
}

Object *dec_ref(Object *obj)
{
    if (obj == NULL) return NULL;
    if (obj->ref_count == 1) {
        debug("releasing object: %s", object_str(obj)->string_val);

        switch (obj->type) {
        case CELL:
            dec_ref(car(obj));
            dec_ref(cdr(obj));
            break;
        case STRING:
            free(obj->string_val);
            break;
        case SYMBOL:
            free(obj->symbol_val);
            break;
        default:
            break;
        }

        free(obj);
        return NULL;
    } else {
        obj->ref_count--;
        return obj;
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

bool symbol_eq(Object *a, Object *b)
{
    if (a->type != SYMBOL || b->type != SYMBOL) {
        return false;
    }
    return strcmp(a->symbol_val, b->symbol_val) == 0;
}

void print_object(Object *obj)
{
    Object *str = object_str(obj);
    puts(str->string_val);
    dec_ref(str);
}

void print_list(Object *obj)
{
    Object *str_obj = list_str(obj);
    puts(str_obj->string_val);
    dec_ref(str_obj);
}

Object *list_str(Object *obj)
{
    Object *obj_str, *list_str_obj;
    size_t obj_str_len, buf_len = 3, written = 0;
    // start with the buffer as 3 bytes - one for each bracket, one for \0
    char *buf = (char *)malloc(buf_len);
    written += snprintf(buf, buf_len, "[");

    int i = 0;
    while (obj != NULL && obj->type == CELL) {
        // for each object in the list, get the string representation of it
        if (car(obj)->type == CELL) {
            obj_str = list_str(car(obj));
        } else {
            obj_str = object_str(car(obj));
        }
        obj_str_len = strlen(obj_str->string_val);
        buf_len += obj_str_len;

        if (i != 0) {
            // increase the buffer length for the comma and space
            buf_len += 2;
        }

        // realloc so we have enough space to add in the object's str
        buf = realloc(buf, buf_len);

        if (i != 0) {
            // add in the comma and space
            written += snprintf(buf + written, 3, ", ");
        }

        // copy in the object's string representation
        strncpy(buf + written, obj_str->string_val, obj_str_len);
        written += obj_str_len;

        // traverse to the next object in the list and continue
        obj = cdr(obj);
        i++;
    }
    // add the trailing bracket - we alloced memory for this at the start
    written += snprintf(buf + written, 2, "]");

    list_str_obj = string_obj(buf);
    free(buf);
    return list_str_obj;
}

Object *object_str(Object *obj)
{
    char *buf;
    int buf_len;
    Object *car_str, *cdr_str, *str_obj;

    if (obj == NULL) {
        return string_obj("null");
    } else {
        switch (obj->type) {
        case CELL:
            car_str = object_str(car(obj));
            cdr_str = object_str(cdr(obj));

            buf_len =  strlen(car_str->string_val);
            buf_len += strlen(cdr_str->string_val);
            buf_len += 5;  // 2 parens + space + comma + \0

            buf = (char *)malloc(buf_len);
            snprintf(buf, buf_len, "(%s, %s)", car_str->string_val,
                                               cdr_str->string_val);

            dec_ref(car_str);
            dec_ref(cdr_str);
            break;
        case INTEGER:
            buf = (char *)malloc(21);  // max 64-bit int = 20 bytes + \0
            snprintf(buf, 21, "%ld", obj->int_val);
            break;
        case FLOAT:
            buf = (char *)malloc(28);  // max formatted double = 28 bytes
            snprintf(buf, 28, "%.20g", obj->float_val);
            break;
        case STRING:
            buf_len = strlen(obj->string_val) + 3;  // strlen + \0 + 2 quotes
            buf = (char *)malloc(buf_len);
            snprintf(buf, buf_len, "\"%s\"", obj->string_val);
            break;
        case SYMBOL:
            buf_len = strlen(obj->symbol_val) + 2;  // strlen + \0 + colon
            buf = (char *)malloc(buf_len);
            snprintf(buf, buf_len, "'%s", obj->symbol_val);
            break;
        default:
            buf = "(Unrecognised Object)";
            break;
        }
        str_obj = string_obj(buf);
        free(buf);
        return str_obj;
    }
}

