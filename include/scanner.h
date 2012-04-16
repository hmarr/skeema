#ifndef _SCANNER_H
#define _SCANNER_H

#include "sk_objects/object.h"

sk_Object *sk_read_token(char **stream_ptr);
sk_Object *sk_read_string_literal(char **stream_ptr);

#endif
