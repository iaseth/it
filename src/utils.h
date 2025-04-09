#ifndef UTILS_H
#define UTILS_H

#include <sys/types.h>
#include <time.h>
#include <stdbool.h>

void format_size(off_t size, char *buf, size_t bufsize);
void format_time_ago(time_t mod_time, char *buf, size_t bufsize);
int strcasecmp_ascii(const char *a, const char *b);
bool is_hidden(const char *name);

#endif
