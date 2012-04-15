#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "scanner.h"
#include "object.h"

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
    if (symbol_eq(token, symbol_obj("("))) {
        // start a new nested list
        obj = tail = NULL;
        while ((next = parse(stream)) != NULL) {
            debug(object_str(next)->string_val);
            if (symbol_eq(next, symbol_obj(")"))) {
                // if we see a closing paren, the nested list is over
                break;
            }

            if (tail == NULL) {
                // this is the first item, create the list
                obj = tail = cons(next, NULL);
            } else {
                // append the new item to the tail and keep track of the tail
                tail = tail->cell_val.cdr = cons(next, NULL);
            }
        }
    } else {
        if (is_integer(token->symbol_val)) {
            obj = int_obj(atol(token->symbol_val));
        } else if (is_float(token->symbol_val)) {
            obj = float_obj(atof(token->symbol_val));
        } else {
            // ordinary symbol, return it as-is
            obj = token;
        }
    }

    return obj;
}

