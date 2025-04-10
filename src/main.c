#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "args.h"
#include "tree.h"

#define MAX_PATH_LENGTH 256
#define MAX_PATHS 10



const char* version = "it v1.0 - Compiled on " __DATE__ " at " __TIME__;

int main(int argc, char *argv[]) {
	struct Args args;
	args.show_hidden = false;
	args.show_simple = false;
	args.show_version = false;

	char paths[MAX_PATHS][MAX_PATH_LENGTH];
	char paths_count = 0;

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			if (isarg(argv[i], "--hidden", "-h")) {
				args.show_hidden = true;
			} else if (isarg(argv[i], "--simple", "-s")) {
				args.show_simple = true;
			} else if (isarg(argv[i], "--version", "-v")) {
				args.show_version = true;
			} else {
				printf("Flag not recognized: '%s'\n", argv[i]);
				return 1;
			}
		} else if (paths_count < MAX_PATHS) {
			strcpy(paths[paths_count], argv[i]);
			paths_count++;
		} else {
			printf("Ignored arg: '%s'\n", argv[i]);
		}
	}

	if (args.show_version) {
		printf("%s\n", version); return 0;
	}

	if (paths_count == 0) {
		printf(".\n");
		print_tree(".", 0, &args);
	} else {
		for (int i = 0; i < paths_count; ++i) {
			printf("%s\n", paths[i]);
			print_tree(paths[i], 0, &args);
		}
	}

	return 0;
}
