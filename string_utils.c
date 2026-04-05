#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

char *ltrim(char *str) {
	if (strlen(str) <= 0)
		return NULL;
	while(isspace(*str)) str++;
	return str;
}

char *rtrim(char *str) {
	if (strlen(str) <= 0)
		return NULL;
	char* back = str + strlen(str);
	while(isspace(*--back));
	*(back + 1) = '\0';
	return str;
}

char *trim(char *str) {
	return rtrim(ltrim(str));
}

// String needs to be freed
char *substr(const char *str, int start, int end) {
	if (str == NULL || start < 0 || end < 0 || start >= end)
		return NULL;

	int len = strlen(str);

	if (start >= len) return strdup("");
	if (end > len) end = len;

	int sublen = end - start;
    char *result = malloc(sublen + 1);

    if (!result) return NULL;

    memcpy(result, str + start, sublen);
    result[sublen] = '\0';
	return result;
}

bool starts_with(const char *str, const char *start) {
	if (str == NULL || start == NULL)
		return false;
	char *temp_str = substr(str, 0, strlen(start));
	bool matches = strcmp(temp_str, start) == 0;
	free(temp_str);
	return matches;
}

bool is_whitespace(const char *str) {
	if (str == NULL)
		return false;
	for (int i = 0; i < strlen(str); i++)
		if (!isspace(str[i]))
			return false;
	return true;
}

