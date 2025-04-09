#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>

#include "ignore.h"
#include "utils.h"

#define MAX_ENTRIES_COUNT 2048

#define COLOR_RESET  "\033[0m"
#define COLOR_BLUE   "\033[1;34m"


bool show_hidden = false;

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

void print_tree(const char *path, int depth) {
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

	for (int i = 0; i < entry_count; i++) {
		struct file_entry *e = &entries[i];

		for (int j = 0; j < depth; j++)
			printf("│\t");

		char time_buf[64];
		format_time_ago(e->st.st_mtime, time_buf, sizeof(time_buf));

		if (S_ISDIR(e->st.st_mode)) {
			printf("├── " COLOR_BLUE "%s" COLOR_RESET " --- %s\n", e->name, time_buf);
			char subpath[4096];
			snprintf(subpath, sizeof(subpath), "%s/%s", path, e->name);
			print_tree(subpath, depth + 1);
		} else {
			char size_buf[32];
			if (e->st.st_size == 0) {
				snprintf(size_buf, sizeof(size_buf), "empty");
			} else {
				format_size(e->st.st_size, size_buf, sizeof(size_buf));
			}
			printf("├── %s --- %s, %s\n", e->name, size_buf, time_buf);
		}
	}
}

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
			print_tree(argv[i], 0);
			printed_any = true;
		}
	}

	if (!printed_any) {
		printf(".\n");
		print_tree(".", 0);
	}

	return 0;
}
