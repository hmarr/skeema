#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"
#include "parser.h"
#include "scanner.h"
#include "sk_objects/string.h"
#include "sk_objects/cell.h"
#include "sk_objects/int.h"
#include "sk_objects/float.h"
#include "sk_objects/symbol.h"

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

sk_Object *sk_parse_list(const sk_VM *vm, char **stream)
{
    // start a new nested list
    sk_Object *next, *obj = NULL, *tail = NULL;
    while ((next = sk_parse(vm, stream)) != NULL) {
        if (sk_object_is(next, sk_StringType) && sk_string_eq_cstr(next, ")")) {
            // if we see a closing paren, the nested list is over
            sk_dec_ref(next);
            break;
        }

        sk_Object *new_tail = sk_cell_new(next, sk_nil);
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

    if (obj == NULL) {
        // empty list is equivalent to nil
        obj = sk_nil;
    }

    return obj;
}

sk_Object *sk_parse(const sk_VM *vm, char **stream)
{
    sk_Object *token, *obj;
    token = sk_read_token(stream);
    debug_obj("read token: %s", token);
    if (token == NULL) {
        error("unexpected end of stream");
        obj = NULL;
    } else if (sk_string_eq_cstr(token, ")")) {
        obj = NULL;
    } else if (sk_string_eq_cstr(token, "'")) {
        // TODO: fix bug with double quote
        sk_Object *expr  = sk_parse(vm, stream),
                  *quote = sk_vm_get_symbol(vm, "quote");
        if (expr == NULL) {
            error("no expression for quoting");
            obj = NULL;
        } else {
            obj = sk_cell_new(quote, sk_nil);
            sk_cell_append(obj, expr);
        }
        sk_dec_ref(expr);
        sk_dec_ref(quote);
    } else if (sk_string_eq_cstr(token, "(")) {
        obj = sk_parse_list(vm, stream);
    } else if (sk_string_eq_cstr(token, "\"")) {
        obj = sk_read_string_literal(stream);
        debug_obj("read string: %s", obj);
    } else {
        if (is_integer(sk_string_cstr(token))) {
            obj = sk_int_new(atol(sk_string_cstr(token)));
        } else if (is_float(sk_string_cstr(token))) {
            obj = sk_float_new(atof(sk_string_cstr(token)));
        } else {
            obj = sk_vm_get_symbol(vm, sk_symbol_cstr(token));
        }
    }
    sk_dec_ref(token);

    return obj;
}

