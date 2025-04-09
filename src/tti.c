#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "tree.h"



bool show_hidden = false;

int main(int argc, char *argv[]) {
	bool printed_any = false;

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-' && argv[i][1] == '-') {
			if (strcmp(argv[i], "--hidden") == 0) {
				show_hidden = true;
			} else {
				printf("Flag not recognized: '%s'\n", argv[i]);
				return 1;
			}
		} else if (argv[i][0] == '-') {
			printf("Flag not recognized: '%s'\n", argv[i]);
			return 1;
		} else {
			if (printed_any)
				printf("\n");
			printf("%s\n", argv[i]);
			print_tree(argv[i], 0, show_hidden);
			printed_any = true;
		}
	}

	if (!printed_any) {
		printf(".\n");
		print_tree(".", 0, show_hidden);
	}

	return 0;
}
