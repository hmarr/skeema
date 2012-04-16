#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "scanner.h"
#include "object.h"

bool symbol_eq(sk_Object *a, char *b)
{
    if (a->type != sk_SYMBOL) {
        return false;
    }
    return strcmp(sk_symbol_cstr(a), b) == 0;
}

bool is_integer(const char *str)
{
    while (*str != '\0') {
        if (!isdigit(*str) && *str != '-') return false;
        str++;
    }
    return true;
}

bool is_float(const char *str)
{
    while (*str != '\0') {
        if (!isdigit(*str) && *str != '.' && *str != '-') return false;
        str++;
    }
    return true;
}

sk_Object *sk_parse(const sk_VM *vm, char **stream)
{
    sk_Object *token, *next, *obj, *tail;
    token = sk_read_symbol(vm, stream);
    if (symbol_eq(token, "(")) {
        // start a new nested list
        obj = tail = NULL;
        while ((next = sk_parse(vm, stream)) != NULL) {
            if (symbol_eq(next, ")")) {
                // if we see a closing paren, the nested list is over
                sk_dec_ref(next);
                break;
            }

            sk_Object *new_tail = sk_cell_new(next, NULL);
            sk_dec_ref(next);
            if (tail == NULL) {
                // this is the first item, create the list
                obj = tail = new_tail;
            } else {
                // append the new item to the tail and keep track of the tail
                sk_cell_set_cdr(tail, new_tail);
                tail = new_tail;
                sk_dec_ref(new_tail);
            }
        }
    } else if (symbol_eq(token, "\"")) {
        obj = sk_read_string_literal(stream);
    } else {
        if (is_integer(sk_symbol_cstr(token))) {
            obj = sk_int_new(atol(sk_symbol_cstr(token)));
        } else if (is_float(sk_symbol_cstr(token))) {
            obj = sk_float_new(atof(sk_symbol_cstr(token)));
        } else {
            obj = sk_vm_get_symbol(vm, sk_symbol_cstr(token));
        }
    }
    sk_dec_ref(token);

    return obj;
}

