#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int name_is_dir(const char *name) {
    size_t n = strlen(name);
    return n && name[n - 1] == '/';
}

int get_entry_parents(const char *path, char ***parents_out) {
	int current = 0, count = 1, now = 0, prev = 0;
	char *_path;

	for (const char *p = path; *p; ++p)
		if (*p == '/') ++count;

	char **parents = malloc(sizeof(char*) * count);

	for (const char *p = path; *p; ++p) {
		if (*p == '/' || now == strlen(path) - 1) {
			if (now == strlen(path) - 1)
				now += 1;

			_path = malloc(sizeof(char*) * (now - prev + 1));
			memcpy(_path, &path[prev], now - prev);
			_path[now - prev] = '\0';

			if (_path[strlen(_path) - 1] == '/') 
				_path[strlen(_path) - 1] = '\0';

			parents[current++] = _path;
			prev = now;
		}
		now++;
	}

	*parents_out = parents;
	return current;
}

void free_parents(char **parents, int parent_count) {
	for (int parent = 0; parent < parent_count; parent++) {
		free(parents[parent]);
	}
}
