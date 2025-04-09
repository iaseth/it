#ifndef ANALYSIS_H
#define ANALYSIS_H

#define LINE_SIZE 1024



struct Analysis {
	int lines;
	int empty_lines;
	int hash_lines;
	int bang_lines;

	int opening_braces;
	int closing_braces;
	int end_colons;
	int end_semicolons;

	int python_imports;
	int python_defs;
	int python_classes;
};

void initialize_analysis_struct(struct Analysis *out);
int do_file_analysis(char *filepath, struct Analysis *out);

#endif
