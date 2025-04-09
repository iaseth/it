#include "analysis.h"

#include <stdio.h>
#include <string.h>

#include "stringutils.h"



void initialize_analysis_struct(struct Analysis *out) {
	out->lines = 0;
	out->empty_lines = 0;
	out->hash_lines = 0;
	out->bang_lines = 0;

	out->double_slash_comments = 0;
	out->slash_star_comments = 0;
	out->xml_tags = 0;

	out->opening_braces = 0;
	out->closing_braces = 0;
	out->end_colons = 0;
	out->end_semicolons = 0;

	out->python_imports = 0;
	out->python_defs = 0;
	out->python_classes = 0;
}

int do_file_analysis(char *filepath, struct Analysis *out) {
	FILE *file = fopen(filepath, "r");
	if (!file) {
		perror("Error opening file");
		return 1;
	}

	initialize_analysis_struct(out);
	char line[LINE_SIZE];

	while (fgets(line, sizeof(line), file)) {
		out->lines++;
		char *start = ltrim(line);
		rtrim(start);

		if (startswith(start, "import ") || startswith(start, "from ")) {
			out->python_imports++;
		} else if (startswith(start, "class ")) {
			out->python_classes++;
		} else if (startswith(start, "def ")) {
			out->python_defs++;
		} else if (startswith(start, "//")) {
			out->double_slash_comments++;
		} else if (startswith(start, "/*")) {
			out->slash_star_comments++;
		} else if (startswith(start, "<") || endswith(start, ">")) {
			out->xml_tags++;
		}

		size_t len = strlen(start);
		if (len == 0) {
			out->empty_lines++;
			continue;
		}

		char first = start[0];
		char last = start[len - 1];

		if (last == ':') out->end_colons++;
		else if (last == ';') out->end_semicolons++;
		else if (last == '{') out->opening_braces++;

		if (first == '#') out->hash_lines++;
		else if (first == '!') out->bang_lines++;
		else if (first == '}') out->closing_braces++;
	}

	fclose(file);
	return 0;
}
