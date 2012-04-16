#include <stdio.h>
#include <stdarg.h>

#include "utils.h"

void debug_with_info(const char *file, int line, const char* fmt, ...)
{
#ifndef DEBUG
    return;
#endif
    va_list args;
    va_start(args, fmt);
    printf("[\033[36mDEBUG\033[0m] (%s:%d) ", file, line);
    vprintf(fmt, args);
    putchar('\n');
    va_end(args);
}

void error_with_info(const char *file, int line, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    printf("[\033[31mERROR\033[0m] (%s:%d) ", file, line);
    vprintf(fmt, args);
    putchar('\n');
    va_end(args);
}

