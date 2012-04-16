#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "utils.h"
#include "object.h"


// Memory management

sk_Object *sk_inc_ref(sk_Object *obj)
{
    if (obj == NULL) return NULL;
    obj->ref_count++;
    return obj;
}

sk_Object *sk_dec_ref(sk_Object *obj)
{
    if (obj == NULL) return NULL;
    if (obj->ref_count == 1) {
        printf("releasing object: ");
        sk_object_debug_print(obj);
        putchar('\n');

        switch (obj->type) {
        case sk_CELL:
            sk_dec_ref(sk_cell_car(obj));
            sk_dec_ref(sk_cell_cdr(obj));
            break;
        case sk_STRING:
            free(((sk_String *)obj)->cstr);
            break;
        case sk_SYMBOL:
            free(((sk_Symbol *)obj)->name_cstr);
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

// Constructor helper
#define sk_new_object_init(T) \
    T *obj = (T *)malloc(sizeof(T)); \
    obj->ref_count = 1;


// Cell functions

sk_Object *sk_cell_new(sk_Object *car, sk_Object *cdr)
{
    sk_new_object_init(sk_Cell)
    obj->type = sk_CELL;
    obj->car = obj->cdr = NULL;
    sk_cell_set_car((sk_Object *)obj, car);
    sk_cell_set_cdr((sk_Object *)obj, cdr);
    return (sk_Object *)obj;
}

void sk_cell_set_car(sk_Object *obj, sk_Object *car)
{
    sk_dec_ref(sk_cell_car(obj));
    ((sk_Cell *)obj)->car = sk_inc_ref(car);
}

void sk_cell_set_cdr(sk_Object *obj, sk_Object *cdr)
{
    sk_dec_ref(sk_cell_cdr(obj));
    ((sk_Cell *)obj)->cdr = sk_inc_ref(cdr);
}


// Int functions

sk_Object *sk_int_new(long value) {
    sk_new_object_init(sk_Int)
    obj->type = sk_INT;
    obj->int_val = value;
    return (sk_Object *)obj;
}


// Float functions

sk_Object *sk_float_new(double value) {
    sk_new_object_init(sk_Float)
    obj->type = sk_FLOAT;
    obj->float_val = value;
    return (sk_Object *)obj;
}


// String functions

sk_Object *sk_string_new(const char *value) {
    sk_new_object_init(sk_String)
    obj->type = sk_STRING;
    obj->cstr = (char *)malloc(strlen(value) + 1);
    strcpy(obj->cstr, value);
    return (sk_Object *)obj;
}


// Symbol functions

sk_Object *sk_symbol_new(const char *name) {
    sk_new_object_init(sk_Symbol)
    obj->type = sk_SYMBOL;
    obj->name_cstr = (char *)malloc(strlen(name) + 1);
    strcpy(obj->name_cstr, name);
    return (sk_Object *)obj;
}


// Utilities

const char *sk_object_type_to_cstr(sk_Object *obj)
{
    if (obj == NULL) return "null";
    switch (obj->type) {
    case sk_CELL:
        return "Cell";
    case sk_INT:
        return "Int";
    case sk_FLOAT:
        return "Float";
    case sk_STRING:
        return "String";
    case sk_SYMBOL:
        return "Symbol";
    default:
        return "unrecognised";
    }
}

void sk_object_debug_print(sk_Object *obj)
{
    switch (obj->type) {
    case sk_CELL:
        printf("<%s (%s %s)>", sk_object_type_to_cstr(obj),
                               sk_object_type_to_cstr(sk_cell_car(obj)),
                               sk_object_type_to_cstr(sk_cell_cdr(obj)));
        break;
    case sk_INT:
        printf("<%s %ld>", sk_object_type_to_cstr(obj), sk_int_val(obj));
        break;
    case sk_FLOAT:
        printf("<%s %f>", sk_object_type_to_cstr(obj), sk_float_val(obj));
        break;
    case sk_STRING:
        printf("<%s \"%s\">", sk_object_type_to_cstr(obj), sk_string_cstr(obj));
        break;
    case sk_SYMBOL:
        printf("<%s '%s>", sk_object_type_to_cstr(obj), sk_symbol_cstr(obj));
        break;
    default:
        printf("<%s>", sk_object_type_to_cstr(obj));
        break;
    }
}

/*
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
        dec_ref(obj_str);

        // traverse to the next object in the list and continue
        obj = cdr(obj);
        i++;
    }
    // add the trailing bracket - we alloced memory for this at the start
    written += snprintf(buf + written, 2, "]");

    list_str_obj = string_obj(buf);
    free(buf);
    return list_str_obj;
}*/

sk_Object *sk_object_to_string(sk_Object *obj)
{
    char *buf;
    int buf_len;

    if (obj == NULL) {
        return sk_string_new("null");
    } else {
        sk_Object *car_str, *cdr_str;

        switch (obj->type) {

        case sk_CELL:
            car_str = sk_object_to_string(sk_cell_car(obj));
            cdr_str = sk_object_to_string(sk_cell_cdr(obj));

            buf_len =  sk_string_length(car_str);
            buf_len += sk_string_length(cdr_str);
            buf_len += 5;  // 2 parens + space + comma + \0

            buf = (char *)malloc(buf_len);
            snprintf(buf, buf_len, "(%s, %s)", sk_string_cstr(car_str),
                                               sk_string_cstr(cdr_str));

            sk_dec_ref(car_str);
            sk_dec_ref(cdr_str);
            break;

        case sk_INT:
            buf = (char *)malloc(21);  // max 64-bit int = 20 bytes + \0
            snprintf(buf, 21, "%ld", sk_int_val(obj));
            break;

        case sk_FLOAT:
            buf = (char *)malloc(28);  // max formatted double = 28 bytes
            snprintf(buf, 28, "%.20g", sk_float_val(obj));
            break;

        case sk_STRING:
            buf_len = sk_string_length(obj) + 3;  // strlen + \0 + 2 quotes
            buf = (char *)malloc(buf_len);
            snprintf(buf, buf_len, "\"%s\"", sk_string_cstr(obj));
            break;

        case sk_SYMBOL:
            buf_len = sk_symbol_length(obj) + 2;  // strlen + \0 + colon
            buf = (char *)malloc(buf_len);
            snprintf(buf, buf_len, "'%s", sk_symbol_cstr(obj));
            break;

        default:
            buf = "(Unrecognised Object)";
            break;
        }

        sk_Object *str_obj = sk_string_new(buf);
        free(buf);

        return str_obj;
    }
}

