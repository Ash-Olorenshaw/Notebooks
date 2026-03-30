#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "./types.h"

/* If len == -1 then the length is inferred.
 * The user is expected to call 'cleanup_str' when done.
 * */
string *create_str(const char *str, int len) {
	string *result = malloc(sizeof(string));

	if (str == NULL) {
		result->val = NULL;
		result->len = 0;
	}
	else {
		char *_str = strdup(str);
		if (len == -1) {
			result->val = _str;
			result->len = strlen(str);
		} 
		else {
			result->val = _str;
			result->len = len;
		}
	}
	return result;
}

void cleanup_str(string *str) {
	if (str != NULL) {
		if (str->val != NULL)
			free(str->val);
		str->val = NULL;
		str->len = 0;
		free(str);
	}
}
