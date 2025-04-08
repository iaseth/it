#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_ENTRIES_COUNT 2048



const char *ignored_dirs[] = {
	"node_modules", ".venv", ".git", "build", "target",
	"__pycache__", "dist", "out", "bin", "obj", "coverage", ".cache"
};
const int num_ignored_dirs = sizeof(ignored_dirs) / sizeof(ignored_dirs[0]);
bool show_hidden = false;

bool should_ignore(const char *name) {
	for (int i = 0; i < num_ignored_dirs; i++) {
		if (strcmp(name, ignored_dirs[i]) == 0)
			return true;
	}
	return false;
}

bool is_hidden(const char *name) {
	return name[0] == '.';
}

void format_size(off_t size, char *buf, size_t bufsize) {
	const char *units[] = {"B", "KB", "MB", "GB", "TB"};
	int i = 0;
	double sz = size;

	while (sz >= 1024 && i < 4) {
		sz /= 1024.0;
		i++;
	}
	snprintf(buf, bufsize, "%.1f %s", sz, units[i]);
}

void format_time_ago(time_t mod_time, char *buf, size_t bufsize) {
	time_t now = time(NULL);
	double diff = difftime(now, mod_time);

	if (diff < 60) {
		snprintf(buf, bufsize, "just now");
	} else if (diff < 3600) {
		int minutes = diff / 60;
		snprintf(buf, bufsize, "%d minute%s ago", minutes, minutes == 1 ? "" : "s");
	} else if (diff < 86400) {
		int hours = diff / 3600;
		snprintf(buf, bufsize, "%d hour%s ago", hours, hours == 1 ? "" : "s");
	} else if (diff < 604800) {
		int days = diff / 86400;
		snprintf(buf, bufsize, "%d day%s ago", days, days == 1 ? "" : "s");
	} else if (diff < 2592000) {
		int weeks = diff / 604800;
		snprintf(buf, bufsize, "%d week%s ago", weeks, weeks == 1 ? "" : "s");
	} else if (diff < 31536000) {
		int months = diff / 2592000;
		snprintf(buf, bufsize, "%d month%s ago", months, months == 1 ? "" : "s");
	} else {
		int years = diff / 31536000;
		snprintf(buf, bufsize, "%d year%s ago", years, years == 1 ? "" : "s");
	}
}

// Helper: lowercase string for case-insensitive sort
int strcasecmp_ascii(const char *a, const char *b) {
	while (*a && *b) {
		char ca = tolower((unsigned char)*a);
		char cb = tolower((unsigned char)*b);
		if (ca != cb) return ca - cb;
		a++; b++;
	}
	return *a - *b;
}

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
			printf("├── %s --- %s\n", e->name, time_buf);
			char subpath[4096];
			snprintf(subpath, sizeof(subpath), "%s/%s", path, e->name);
			print_tree(subpath, depth + 1);
		} else {
			char size_buf[32];
			format_size(e->st.st_size, size_buf, sizeof(size_buf));
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
