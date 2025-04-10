#ifndef ARGS_H
#define ARGS_H

#include <stdbool.h>


struct Args {
	bool show_hidden;
	bool show_simple;
};

bool isarg(const char *arg, const char *long_name, const char *short_name);

#endif
