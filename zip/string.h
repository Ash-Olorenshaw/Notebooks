#ifndef ZIP_STRING_H
#define ZIP_STRING_H

int name_is_dir(const char *name);
int get_entry_parents(const char *path, char ***parents_out);
void free_parents(char **parents, int parent_count);

#endif
