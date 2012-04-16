#ifndef _UTILS_H
#define _UTILS_H

#include "sk_objects/object.h"

void debug_with_info(const char *file, int line, const char* fmt, ...);
void error_with_info(const char *file, int line, const char* fmt, ...);

#define debug(...) do {\
    debug_with_info(__FILE__, __LINE__, __VA_ARGS__);\
} while (0)

#define debug_obj(FMT, OBJ) do {\
    sk_Object *_o_str = sk_object_to_string(OBJ);\
    debug_with_info(__FILE__, __LINE__, FMT, sk_string_cstr(_o_str));\
    sk_dec_ref(_o_str);\
} while (0)

#define error(...) do {\
    error_with_info(__FILE__, __LINE__, __VA_ARGS__);\
} while (0)

#define error_obj(FMT, OBJ) do {\
    sk_Object *_o_str = sk_object_to_string(OBJ);\
    error_with_info(__FILE__, __LINE__, FMT, sk_string_cstr(_o_str));\
    sk_dec_ref(_o_str);\
} while (0)

#endif
