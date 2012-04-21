#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "utils.h"
#include "scanner.h"
#include "sk_objects/string.h"

#define STR_BUFFER_CHUNK_SIZE (32)

bool is_delim(char c)
{
    return isspace(c) || c == '(' || c == ')' || c == '\0';
}

sk_Object *sk_read_token(char **stream_ptr)
{
    sk_Object *token = NULL;
    int token_len = 1;

    // skip whitespace to start reading a token
    while (isspace(**stream_ptr)) (*stream_ptr)++;

    // check we're not at the end of the stream
    if (**stream_ptr == '\0') {
        return NULL;
    }

    if (**stream_ptr == '(' || **stream_ptr == ')'||
        **stream_ptr == '"' || **stream_ptr == '\'') {
        // return parens and double quotes individually
        token_len = 1;
    } else {
        // find out token size
        while (!is_delim(*(*stream_ptr + token_len))) token_len++;
    }

    // copy the token from the stream to a buffer
    char *buf = (char *)malloc(token_len + 1);
    memset(buf, 0, token_len + 1);
    strncpy(buf, *stream_ptr, token_len);
    // wrap the token up as a string
    token = sk_string_new(buf);
    free(buf);

    // advance the stream pointer for next time
    *stream_ptr += token_len;

    return token;
}

sk_Object *sk_read_string_literal(char **stream_ptr)
{
    bool escape = false;
    int buf_size = STR_BUFFER_CHUNK_SIZE, i = 0;
    char c, *buf = (char *)malloc(buf_size);

    do {
        c = **stream_ptr;
        if (!escape && c == '"') break;  // stop if we hit the final quote

        // if we hit an unescaped backslash, it's escaping something else
        escape = c == '\\' && !escape;
        if (escape) continue;  // escape chars are not part of the string

        if (i == buf_size - 1) {
            debug("buffer too small (%d), reallocing", buf_size);
            buf_size += STR_BUFFER_CHUNK_SIZE;
            buf = realloc(buf, buf_size);
        }

        buf[i++] = c;  // copy character to temp buffer
    } while ((*stream_ptr)++);

    (*stream_ptr)++;  // move over the final quote

    buf[i] = '\0';  // terminate the string

    sk_Object *string = sk_string_new(buf);
    free(buf);

    return string;
}

