#include <stdlib.h>
#include <stdbool.h>
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
	FILE *fp = fopen("notebook.nn", "rb");
    if (!fp) { perror("fopen"); return NULL; }
    fseek(fp, 0, SEEK_END);
    long sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    zip_buf = malloc(sz);
    fread(zip_buf, 1, sz, fp);
    fclose(fp);

    zip_error_t zerr;
    zip_source_t *src = zip_source_buffer_create(zip_buf, (zip_uint64_t)sz, 0, &zerr);
    if (!src) {
        fprintf(stderr, "zip_source_buffer_create: %s\n", zip_error_strerror(&zerr));
        free(zip_buf);
        return NULL;
    }

    za = zip_open_from_source(src, ZIP_RDONLY, &zerr);
    if (!za) {
        fprintf(stderr, "zip_open_from_source: %s\n", zip_error_strerror(&zerr));
        zip_source_free(src);
        free(zip_buf);
        return NULL;
    }

	NotebookEntry *root_node = create_entry(NULL, create_str("/", -1), create_str(NULL, -1));
	root_node->is_folder = true;
    walk(za, root_node);

    return root_node;
}

int cleanup_archive(NotebookEntry *root_node) {
    zip_close(za);
    free(zip_buf);
	return 0;
}
