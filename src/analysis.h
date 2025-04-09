#ifndef ANALYSIS_H
#define ANALYSIS_H

#define LINE_SIZE 1024



struct Analysis {
	int lines;
	int empty_lines;
	int hash_lines;

	int opening_braces;
	int closing_braces;
	int end_semicolons;
	int end_colons;

	int python_defs;
	int python_classes;
	int python_imports;
};

void initialize_analysis_struct(struct Analysis *out);
int analysis_file(char *filepath, struct Analysis *out);

#endif
