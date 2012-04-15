#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <stdarg.h>

void debug(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    printf("[DEBUG] ");
    vprintf(fmt, args);
    va_end(args);
}

void error(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    printf("[ERROR] ");
    vprintf(fmt, args);
    va_end(args);
}

#endif
