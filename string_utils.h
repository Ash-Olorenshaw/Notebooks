#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stdbool.h>

char *trim(char *str);
char *rtrim(char *str);
char *ltrim(char *str);

char *substr(const char *str, int start, int end);
bool starts_with(const char *str, const char *start);
bool is_whitespace(const char *str);

#endif
