#ifndef TREE_H
#define TREE_H

#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>


struct file_entry {
	char name[256];
	struct stat st;
};

int compare_entries(const void *a, const void *b);
void add_file_attributes(char *subpath, struct file_entry *entry, char *text_buf);
void print_tree(const char *path, int depth, bool show_hidden, bool show_simple);

#endif
