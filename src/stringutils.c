#include "stringutils.h"

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
