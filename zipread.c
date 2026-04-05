#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <zip.h>

#include "./zipread.h"
#include "./zip/string.h"
#include "./zip/entries.h"

zip_t *za;
void *zip_buf;

static void print_full_tree(const NotebookEntry *node, const char *prefix, bool is_last) {
    printf("%s%s── %s%s\n",
           prefix,
           is_last ? "└" : "├",
           node->name->val,
           node->is_folder ? "/" : "");

    char new_prefix[256];
    snprintf(new_prefix, sizeof(new_prefix), "%s%s",
             prefix,
             is_last ? "    " : "│   ");

    for (int i = 0; i < node->child_count; ++i) {
        bool last_child = (i == node->child_count - 1);
        print_full_tree(node->children[i], new_prefix, last_child);
    }
}

static void walk(zip_t *za, NotebookEntry *root) {
    zip_int64_t n = zip_get_num_entries(za, 0);
    for (zip_int64_t i = 0; i < n; ++i) {
        struct zip_stat st;
        if (zip_stat_index(za, i, 0, &st) != 0) {
            fprintf(stderr, "zip_stat_index(%lld) failed\n", (long long)i);
            continue;
        }
        int depth = 0;
		char **parents;
		int parent_count = get_entry_parents(st.name, &parents);
		NotebookEntry *current_book = root;

		for (int parent = 0; parent < parent_count - 1; parent++) {
			get_entry(current_book, parents[parent], &current_book, (get_entry_opts){ .create_new = true, .is_folder = true });
		}

		NotebookEntry *new_entry = create_entry(current_book, create_str(parents[parent_count - 1], -1), create_str(st.name, -1));
		free_parents(parents, parent_count);
		free(parents);
		new_entry->is_folder = name_is_dir(st.name);
    }
	print_full_tree(root, "", true);
}

NotebookEntry *read_archive() {
    int zerr;
    za = zip_open("notebook.nn", 0, &zerr);

	NotebookEntry *root_node = create_entry(NULL, create_str("/", -1), create_str(NULL, -1));
	root_node->is_folder = true;
    walk(za, root_node);

    return root_node;
}

char *read_zip_file(char *file) {
	struct zip_file *f = zip_fopen(za, file, 0);
	if (f == NULL)
		return NULL;

	struct zip_stat stat;
	if (zip_stat(za, file, 0, &stat) == -1)
		return NULL;

	char *content = malloc(sizeof(char) * (stat.size + 1));
	if (zip_fread(f, content, stat.size) == -1)
		return NULL;
	content[stat.size] = '\0';

	zip_fclose(f);

	return content;
}

int write_zip_file(char *file, char *content) {
	struct zip_file *f = zip_fopen(za, file, 0);
	if (f == NULL)
		return -1;

	struct zip_error err;
	struct zip_source *zip_content = zip_source_buffer_create(content, strlen(content), true, &err);
	if (zip_content == NULL) {
		printf("ZIP WRITE ERR:\n%s\n%d\n%s (%d)\n", err.str, err.sys_err, err.zip_err == ZIP_ER_INVAL ? "ZIP_ER_INVAL" : err.zip_err == ZIP_ER_MEMORY ? "ZIP_ER_MEMORY" : "unknown err...", err.zip_err);
		return -1;
	}

	int f_index = zip_name_locate(za, file, 0);
	if (f_index == -1) {
		if (zip_file_add(za, file, zip_content, 0) == -1) {
			printf("ZIP FILE CREATION ERR:\n%s\n", zip_strerror(za));
			zip_source_free(zip_content);
			return -1;
		}
	}
	else {
		if (zip_file_replace(za, f_index, zip_content, 0) == -1) {
			zip_source_free(zip_content);
			printf("ZIP FILE SAVE ERR:\n%s\n", zip_strerror(za));
			return -1;
		}
	}

	zip_fclose(f);
	return 0;
}

int cleanup_archive(NotebookEntry *root_node) {
    zip_close(za);
    free(zip_buf);
	return 0;
}
