#ifndef TREE_H
#define TREE_H

#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>

#include "args.h"



struct file_entry {
	char name[256];
	struct stat st;
};

bool is_code_file(const char *filename);
void add_file_attributes(char *subpath, struct file_entry *entry, char *text_buf);
int compare_entries(const void *a, const void *b);
void print_tree(const char *path, int depth, struct Args *args);

#endif
