#include <stdlib.h>
#include <string.h>

#include "../types.h"
#include "./entries.h"

NotebookEntry *create_entry(NotebookEntry *parent, string *entry_name, string *zip_name) {
    NotebookEntry *child = malloc(sizeof *child);
    if (!child) return NULL;

    child->name = entry_name;
    child->zip_file_name = zip_name;
    child->child_count = 0;
    child->max_child = 0;
    child->children = NULL;

    if (parent) {
        if (parent->child_count == parent->max_child) {
            int new_cap = parent->max_child ? parent->max_child * 2 : 4;
            NotebookEntry **new_block = realloc(parent->children, new_cap * sizeof *new_block);

            if (!new_block) { 
				free(child); 
				return NULL; 
			}

            parent->children = new_block;
            parent->max_child  = new_cap;
        }
        parent->children[parent->child_count++] = child;
    }
    return child;
}

void free_entry(NotebookEntry *entry) {
    if (!entry) return;

    cleanup_str(entry->name);
    cleanup_str(entry->zip_file_name);

    for (int i = 0; i < entry->child_count; ++i)
        free_entry(entry->children[i]);

    free(entry->children);
    free(entry);
}

int get_entry(NotebookEntry *parent, const char *entry_name, NotebookEntry **result, get_entry_opts opts) {
	for (int i = 0; i < parent->child_count; i++) {
		if (strcmp(parent->children[i]->name->val, entry_name) == 0) {
			*result = parent->children[i];
			return 0;
		}
	}

	if (opts.create_new) {
		*result = create_entry(parent, create_str(entry_name, -1), create_str(NULL, -1));
		(*result)->is_folder = opts.is_folder;
		return 0;
	}

	return 1;
}
