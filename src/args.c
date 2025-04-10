#include "args.h"

#include <string.h>



bool isarg(const char *arg, const char *long_name, const char *short_name) {
	if (strcmp(arg, long_name) == 0 || strcmp(arg, short_name) == 0) {
		return true;
	}
	return false;
}
