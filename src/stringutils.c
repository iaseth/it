#include "stringutils.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>



// Trim leading whitespace
char *ltrim(char *str) {
	while (isspace((unsigned char)*str)) str++;
	return str;
}

// Trim trailing whitespace
char *rtrim(char *str) {
	int len = strlen(str);
	while (len > 0 && isspace((unsigned char)str[len - 1])) {
		str[--len] = '\0';
	}
	return str;
}

// Check if a string starts with a given prefix
bool startswith(const char *str, const char *prefix) {
	return strncmp(str, prefix, strlen(prefix)) == 0;
}

// Check if a string ends with a given suffix
bool endswith(const char *str, const char *suffix) {
	size_t str_len = strlen(str);
	size_t suffix_len = strlen(suffix);
	if (suffix_len > str_len) return 0;
	return strcmp(str + str_len - suffix_len, suffix) == 0;
}

// Appends formatted output to the end of an existing string buffer
void append(char *dest, const char *format, ...) {
	// Find the current length of the destination string
	size_t len = strlen(dest);

	// Start processing variable arguments
	va_list args;
	va_start(args, format);

	// Append formatted output starting at the end of the current string
	vsprintf(dest + len, format, args);

	va_end(args);
}
