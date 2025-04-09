#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>



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

bool is_hidden(const char *name) {
	return name[0] == '.';
}


int count_entries_in_dir(DIR *dir) {
	struct dirent *entry;
	int count = 0;

	while ((entry = readdir(dir)) != NULL) {
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;
		count++;
	}

	rewinddir(dir); // Reset directory stream for reuse
	return count;
}
