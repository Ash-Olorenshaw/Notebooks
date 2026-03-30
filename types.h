#ifndef TYPES_H
#define TYPES_H

typedef struct {
	char *val;
	int len;
} string;

string *create_str(const char *str, int len);
void cleanup_str(string *str);

#endif
