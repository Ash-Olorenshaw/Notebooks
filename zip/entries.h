#ifndef ZIP_ENTRIES_H
#define ZIP_ENTRIES_H

#include <stdbool.h>
#include "../types.h"

typedef struct NotebookEntry {
	string *name;
	string *zip_file_name;
	int max_child, child_count;
	bool is_folder;
	struct NotebookEntry **children;
	char *content;
} NotebookEntry;

NotebookEntry *create_entry(NotebookEntry *parent, string *entry_name, string *zip_name);
void free_entry(NotebookEntry *entry);

typedef struct {
	bool create_new;
	bool is_folder;
} get_entry_opts;

int get_entry(NotebookEntry *parent, const char *entry_name, NotebookEntry **result, get_entry_opts opts);

#endif
