#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <stdarg.h>

void debug(const char* fmt, ...)
{
#ifndef DEBUG
    return;
#endif
    va_list args;
    va_start(args, fmt);
    printf("[\033[36mDEBUG\033[0m] ");
    vprintf(fmt, args);
    putchar('\n');
    va_end(args);
}

void error(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    printf("[\033[31mERROR\033[0m] ");
    vprintf(fmt, args);
    putchar('\n');
    va_end(args);
}

#endif
