#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utils.h"
#include "scanner.h"

bool is_space(char c)
{
    return c == ' ' || c == '\t' || c == '\n';
}

bool is_delim(char c)
{
    return is_space(c) || c == '(' || c == ')' || c == '\0';
}

Object *read_token(char **stream_ptr)
{
    Object *token = NULL;
    int token_len = 1;

    // skip whitespace to start reading a token
    while (is_space(**stream_ptr)) (*stream_ptr)++;

    // check we're not at the end of the stream
    if (**stream_ptr == '\0') {
        return NULL;
    }

    if (**stream_ptr == '(' || **stream_ptr == ')') {
        // return parens individually
        token_len = 1;
    } else {
        // find out token size
        while (!is_delim(*(*stream_ptr + token_len))) token_len++;
    }

    // copy the token from the stream to a buffer
    char *buf = (char *)malloc(token_len + 1);
    memset(buf, 0, token_len + 1);
    strncpy(buf, *stream_ptr, token_len);
    // wrap the token up as a symbol
    token = symbol_obj(buf);
    free(buf);

    // advance the stream pointer for next time
    *stream_ptr += token_len;

    return token;
}

