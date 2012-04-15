#ifndef _SCANNER_H
#define _SCANNER_H

#include "object.h"

Object *read_token(char **stream_ptr);
Object *read_string_literal(char **stream_ptr);

#endif
