#include <stdlib.h>
#include <string.h>

#include "scanner.h"
#include "object.h"

Object *parse(char **stream)
{
    Object *token, *next, *list, *tail;
    token = read_token(stream);
    if (symbol_eq(token, symbol_obj("("))) {
        // start a new nested list
        list = tail = NULL;
        while ((next = parse(stream)) != NULL) {
            if (symbol_eq(next, symbol_obj(")"))) {
                // if we see a closing paren, the nested list is over
                break;
            }

            if (tail == NULL) {
                // this is the first item, create the list
                list = tail = cons(next, NULL);
            } else {
                // append the new item to the tail and keep track of the tail
                tail = tail->cell_val.cdr = cons(next, NULL);
            }
        }
        return list;
    } else {
        // ordinary symbol, return it as-is
        return token;
    }
}

