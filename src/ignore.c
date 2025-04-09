#include <string.h>
#include <stdbool.h>
#include "ignore.h"



const char *ignored_dirs[] = {
	"node_modules", ".venv", ".git", "build", "target",
	"__pycache__", "dist", "out", "bin", "obj", "coverage", ".cache"
};

const int num_ignored_dirs = sizeof(ignored_dirs) / sizeof(ignored_dirs[0]);

bool should_ignore(const char *name) {
	for (int i = 0; i < num_ignored_dirs; i++) {
		if (strcmp(name, ignored_dirs[i]) == 0)
			return true;
	}
	return false;
}
