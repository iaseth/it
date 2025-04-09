#include "tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include "analysis.h"
#include "stringutils.h"
#include "utils.h"
#include "ignore.h"

#define MAX_ENTRIES_COUNT 2048

#define COLOR_RESET  "\033[0m"
#define COLOR_BLUE   "\033[1;34m"



const int MAX_FILE_SIZE_FOR_ANALYSIS = 32 * 1024; // 32 KB

struct file_entry {
	char name[256];
	struct stat st;
};

int compare_entries(const void *a, const void *b) {
	struct file_entry *ea = (struct file_entry *)a;
	struct file_entry *eb = (struct file_entry *)b;
	int is_dir_a = S_ISDIR(ea->st.st_mode);
	int is_dir_b = S_ISDIR(eb->st.st_mode);

	if (is_dir_a != is_dir_b)
		return is_dir_b - is_dir_a; // dirs first

	return strcasecmp_ascii(ea->name, eb->name);
}

void print_tree(const char *path, int depth, bool show_hidden) {
	DIR *dir = opendir(path);
	if (!dir) {
		perror("opendir");
		return;
	}

	struct dirent *entry;
	struct file_entry entries[MAX_ENTRIES_COUNT];
	int entry_count = 0;

	while ((entry = readdir(dir)) != NULL && entry_count < MAX_ENTRIES_COUNT) {
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;

		if (!show_hidden && is_hidden(entry->d_name))
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
	bool is_dir = false;
	struct Analysis analysis;

	for (int i = 0; i < entry_count; i++) {
		struct file_entry *e = &entries[i];

		for (int j = 0; j < depth; j++)
			printf("│\t");

		snprintf(subpath, sizeof(subpath), "%s/%s", path, e->name);
		format_time_ago(e->st.st_mtime, time_buf, sizeof(time_buf));
		is_dir = S_ISDIR(e->st.st_mode);

		if (is_dir) {
			printf("├── " COLOR_BLUE "%s" COLOR_RESET " --- %s\n", e->name, time_buf);
			print_tree(subpath, depth + 1, show_hidden);
		} else {
			printf("├── %s --- %s", e->name, time_buf);
			if (e->st.st_size > MAX_FILE_SIZE_FOR_ANALYSIS) {
				format_size(e->st.st_size, size_buf, sizeof(size_buf));
				printf(", %s", size_buf);
			} else if (endswith(e->name, ".gitignore")) {
				do_file_analysis(subpath, &analysis);
				int non_empty_lines = analysis.lines - analysis.empty_lines;
				int rules = non_empty_lines - analysis.hash_lines - analysis.bang_lines;
				printf(", %d entries, %d overrides", rules, analysis.bang_lines);
			} else if (endswith(e->name, ".c") || endswith(e->name, ".h")
					|| endswith(e->name, ".cpp") || endswith(e->name, ".hpp")) {
				do_file_analysis(subpath, &analysis);
				printf(", %d hashlines, %d statements", analysis.hash_lines, analysis.end_semicolons);
			} else if (endswith(e->name, ".py")) {
				do_file_analysis(subpath, &analysis);
				printf(", %d blocks, %d defs", analysis.end_colons, analysis.python_defs);
			} else if (endswith(e->name, ".md")) {
				do_file_analysis(subpath, &analysis);
				printf(", %d headers", analysis.hash_lines);
			} else {
				if (e->st.st_size == 0) {
					snprintf(size_buf, sizeof(size_buf), "empty");
				} else {
					format_size(e->st.st_size, size_buf, sizeof(size_buf));
				}
				printf(", %s", size_buf);
			}
			printf("\n");
		}
	}
}
