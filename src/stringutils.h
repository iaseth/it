#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <stdarg.h>
#include <stdbool.h>



char *ltrim(char *str);
char *rtrim(char *str);

bool startswith(const char *str, const char *prefix);
bool endswith(const char *str, const char *suffix);

void append(char *dest, const char *format, ...);

#endif
