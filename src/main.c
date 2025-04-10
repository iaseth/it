#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "args.h"
#include "tree.h"



int main(int argc, char *argv[]) {
	struct Args args;
	args.show_hidden = false;
	args.show_simple = false;
	bool printed_any = false;

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			if (isarg(argv[i], "--hidden", "-h")) {
				args.show_hidden = true;
			} else if (isarg(argv[i], "--simple", "-s")) {
				args.show_simple = true;
			} else {
				printf("Flag not recognized: '%s'\n", argv[i]);
				return 1;
			}
		} else {
			if (printed_any)
				printf("\n");
			printf("%s\n", argv[i]);
			print_tree(argv[i], 0, &args);
			printed_any = true;
		}
	}

	if (!printed_any) {
		printf(".\n");
		print_tree(".", 0, &args);
	}

	return 0;
}
