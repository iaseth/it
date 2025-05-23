#include "tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "analysis.h"
#include "stringutils.h"
#include "utils.h"
#include "ignore.h"

#define MAX_ENTRIES_COUNT 2048

#define COLOR_RESET  "\033[0m"
#define COLOR_BLUE   "\033[1;34m"



const int MAX_FILE_SIZE_FOR_ANALYSIS = 64 * 1024; // 64 KB

// List of recognized code file extensions
const char *RECOGNIZED_FILE_TYPES[] = {
	".c", ".cpp", ".h", ".hpp", ".py", ".java", ".js", ".ts", ".cs",
	".rb", ".go", ".swift", ".php", ".rs", ".kt", ".m", ".scala",
	".css", ".html", ".xhtml", ".xml",
	".md", ".txt"
};

const char *RECOGNIZED_FILE_NAMES[] = {
	"README", "LICENSE"
};

bool is_code_file(const char *filename) {
	size_t names_count = sizeof(RECOGNIZED_FILE_NAMES) / sizeof(RECOGNIZED_FILE_NAMES[0]);
	for (size_t i = 0; i < names_count; i++) {
		if (strcmp(filename, RECOGNIZED_FILE_NAMES[i]) == 0) {
			return true;
		}
	}

	const char *dot = strrchr(filename, '.'); // Find the last dot in the filename
	if (!dot || dot == filename) {
		return false; // No extension found
	}

	size_t types_count = sizeof(RECOGNIZED_FILE_TYPES) / sizeof(RECOGNIZED_FILE_TYPES[0]);
	for (size_t i = 0; i < types_count; i++) {
		if (strcmp(dot, RECOGNIZED_FILE_TYPES[i]) == 0) {
			return true;
		}
	}

	return false;
}

void add_file_attributes(char *subpath, struct file_entry *entry, char *text_buf) {
	char *filename = entry->name;
	struct Analysis analysis;
	do_file_analysis(subpath, &analysis);

	if (endswith(filename, ".gitignore")) {
		int non_empty_lines = analysis.lines - analysis.empty_lines;
		int rules = non_empty_lines - analysis.hash_lines - analysis.bang_lines;
		append_attribute(text_buf, "rules", rules);
		append_attribute(text_buf, "overrides", analysis.bang_lines);
	} else if (endswith(filename, ".c") || endswith(filename, ".h")
			|| endswith(filename, ".cpp") || endswith(filename, ".hpp")) {
		append_attribute(text_buf, "hashlines", analysis.hash_lines);
		append_attribute(text_buf, "blocks", analysis.closing_braces);
		append_attribute(text_buf, "statements", analysis.end_semicolons);
		append_attribute(text_buf, "comments", analysis.double_slash_comments + analysis.slash_star_comments);
	} else if (endswith(filename, ".css") || endswith(filename, ".scss")) {
		append_attribute(text_buf, "blocks", analysis.closing_braces);
		append_attribute(text_buf, "styles", analysis.end_semicolons);
		append_attribute(text_buf, "comments", analysis.double_slash_comments);
	} else if (endswith(filename, ".html") || endswith(filename, ".xhtml")
			|| endswith(filename, ".xml") || endswith(filename, ".php")) {
		append_attribute(text_buf, "tags", analysis.xml_tags);
	} else if (endswith(filename, ".js") || endswith(filename, ".ts")) {
		append_attribute(text_buf, "blocks", analysis.closing_braces);
		append_attribute(text_buf, "statements", analysis.end_semicolons);
		append_attribute(text_buf, "comments", analysis.double_slash_comments);
	} else if (endswith(filename, ".py")) {
		append_attribute(text_buf, "blocks", analysis.end_colons);
		append_attribute(text_buf, "defs", analysis.python_defs);
	} else if (endswith(filename, ".md")) {
		append_attribute(text_buf, "headers", analysis.hash_lines);
	} else if (endswith(filename, ".txt")) {
		append_attribute(text_buf, "lines", analysis.lines);
	} else {
		char size_buf[32];
		if (entry->st.st_size == 0) {
			snprintf(size_buf, sizeof(size_buf), "empty");
		} else {
			format_size(entry->st.st_size, size_buf, sizeof(size_buf));
		}
		append(text_buf, ", %s", size_buf);
	}
}

int compare_entries(const void *a, const void *b) {
	struct file_entry *ea = (struct file_entry *)a;
	struct file_entry *eb = (struct file_entry *)b;
	int is_dir_a = S_ISDIR(ea->st.st_mode);
	int is_dir_b = S_ISDIR(eb->st.st_mode);

	if (is_dir_a != is_dir_b)
		return is_dir_b - is_dir_a; // dirs first

	return strcasecmp_ascii(ea->name, eb->name);
}

void print_tree(const char *path, int depth, struct Args *args) {
	DIR *dir = opendir(path);
	if (!dir) {
		perror("opendir");
		return;
	}

	struct dirent *entry;
	int allocated_entry_count = count_entries_in_dir(dir);
	allocated_entry_count = (allocated_entry_count > MAX_ENTRIES_COUNT) ? MAX_ENTRIES_COUNT : allocated_entry_count;

	struct file_entry *entries = malloc(sizeof(struct file_entry) * allocated_entry_count);
	if (!entries) {
		perror("malloc");
		closedir(dir);
		return;
	}

	int entry_count = 0;

	while ((entry = readdir(dir)) != NULL && entry_count < allocated_entry_count) {
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;

		if (!args->show_hidden && is_hidden(entry->d_name))
			continue;

		if (should_ignore(entry->d_name))
			continue;

		char fullpath[4096];
		snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

		struct stat st;
		if (stat(fullpath, &st) != 0)
			continue;

		strncpy(entries[entry_count].name, entry->d_name, sizeof(entries[entry_count].name) - 1);
		entries[entry_count].name[sizeof(entries[entry_count].name) - 1] = '\0';
		entries[entry_count].st = st;
		entry_count++;
	}

	closedir(dir);

	qsort(entries, entry_count, sizeof(struct file_entry), compare_entries);

	char subpath[4096];
	char time_buf[64];
	char size_buf[32];
	char text_buf[512];
	bool is_dir = false;

	for (int i = 0; i < entry_count; i++) {
		struct file_entry *e = &entries[i];

		for (int j = 0; j < depth; j++)
			printf("│\t");

		snprintf(subpath, sizeof(subpath), "%s/%s", path, e->name);
		format_time_ago(e->st.st_mtime, time_buf, sizeof(time_buf));
		is_dir = S_ISDIR(e->st.st_mode);

		if (is_dir) {
			printf("├── " COLOR_BLUE "%s" COLOR_RESET " --- %s\n", e->name, time_buf);
			print_tree(subpath, depth + 1, args);
		} else {
			text_buf[0] = '\0';
			printf("├── %s --- %s", e->name, time_buf);
			bool do_analysis = (!args->show_simple) && (e->st.st_size < MAX_FILE_SIZE_FOR_ANALYSIS) && is_code_file(e->name);
			if (do_analysis) {
				add_file_attributes(subpath, e, text_buf);
			} else {
				format_size(e->st.st_size, size_buf, sizeof(size_buf));
				append(text_buf, ", %s", size_buf);
			}
			printf("%s\n", text_buf);
		}
	}

	free(entries);
}
