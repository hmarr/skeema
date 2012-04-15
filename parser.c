#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "scanner.h"
#include "object.h"

bool symbol_eq(Object *a, char *b)
{
    if (a->type != SYMBOL) {
        return false;
    }
    return strcmp(a->symbol_val, b) == 0;
}

bool is_integer(const char *str)
{
    while (*str != '\0') {
        if (!isdigit(*(str++))) return false;
    }
    return true;
}

bool is_float(const char *str)
{
    while (*str != '\0') {
        if (!isdigit(*str) && *str != '.') return false;
        str++;
    }
    return true;
}

Object *parse(char **stream)
{
    Object *token, *next, *obj, *tail;
    token = read_token(stream);
    if (symbol_eq(token, "(")) {
        // start a new nested list
        obj = tail = NULL;
        while ((next = parse(stream)) != NULL) {
            if (symbol_eq(next, ")")) {
                // if we see a closing paren, the nested list is over
                dec_ref(next);
                break;
            }

            Object *new_tail = cons(next, NULL);
            dec_ref(next);
            if (tail == NULL) {
                // this is the first item, create the list
                obj = tail = new_tail;
            } else {
                // append the new item to the tail and keep track of the tail
                set_cdr(tail, new_tail);
                tail = new_tail;
                dec_ref(new_tail);
            }
        }
    } else if (symbol_eq(token, "\"")) {
        obj = read_string_literal(stream);
    } else {
        if (is_integer(token->symbol_val)) {
            obj = int_obj(atol(token->symbol_val));
        } else if (is_float(token->symbol_val)) {
            obj = float_obj(atof(token->symbol_val));
        } else {
            obj = symbol_obj(token->symbol_val);
        }
    }
    dec_ref(token);

    return obj;
}

