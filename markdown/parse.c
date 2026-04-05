#include <ctype.h>

#include "../string_utils.h"
#include "./setup.h"
#include "./utils.h"
#include "../arrays.h"

typedef struct {
	int start, end;
} text_section;

void apply_sub_styles(const char *line, int line_len, GtkTextBuffer *text_buffer) {
	int i = 0, deletions_ptr = 0, used_tkns_ptr = 0, pos;
	text_section deletions[50] = {0};
	char next_char, prev_char, prev_prev_char, c;
	rarray *used_tkns = rarray_create(100, sizeof(int));
	GtkTextIter start_iter, end_iter;
	GtkTextIter *start_iter_ptr = &start_iter,
				*end_iter_ptr = &end_iter;

	gtk_text_buffer_get_end_iter(text_buffer, start_iter_ptr);
	gtk_text_buffer_get_end_iter(text_buffer, end_iter_ptr);

	while (i <= line_len) {
		next_char = ' ';
		prev_char = ' ';
		prev_prev_char = ' ';
		c = line[i];

		if (i > 1)
			prev_char = line[i-1];
		if (i > 2)
			prev_prev_char = line[i-2];
		if (i < strlen(line))
			next_char = line[i+1];

		rarray *current_tkn = rarray_create(1, sizeof(char *));
		rarray_add(current_tkn, &i);
		if (tkns_defined(current_tkn, used_tkns)) {
			i++;
			continue;
		}

		if (c == '*' && prev_char == '*' && prev_prev_char == '*') {
			for (int j = i + 1; j < line_len; j++) {
				next_char = ' ';
				prev_char = ' ';
				prev_prev_char = ' ';
				c = line[j];

				current_tkn = rarray_create(1, sizeof(char *));
				rarray_add(current_tkn, &i);

				if (j > 1)
					prev_char = line[j-1];
				if (j > strlen(line))
					next_char = line[j+1];
				if (tkns_defined(current_tkn, used_tkns)) {
					i++;
					continue;
				}

				if (c == '*' && prev_char == '*' && next_char == '*') {
					gtk_text_iter_forward_to_end(start_iter_ptr);
					gtk_text_iter_forward_to_end(end_iter_ptr);
					pos = line_len - i;
                    gtk_text_iter_backward_chars(start_iter_ptr, pos);
					pos = line_len - j;
					gtk_text_iter_backward_chars(end_iter_ptr, pos);
					gtk_text_buffer_apply_tag(text_buffer, bold_tag, start_iter_ptr, end_iter_ptr);
					gtk_text_buffer_apply_tag(text_buffer, italics_tag, start_iter_ptr, end_iter_ptr);

					deletions[deletions_ptr++] = (text_section){ .start = i - 3, .end = i };
					deletions[deletions_ptr++] = (text_section){ .start = j - 2, .end = j + 1 };

					rarray_add(used_tkns, MALLOC_CREATE(int, i));
					rarray_add(used_tkns, MALLOC_CREATE(int, i-1));
					rarray_add(used_tkns, MALLOC_CREATE(int, i-2));
					rarray_add(used_tkns, MALLOC_CREATE(int, j));
					rarray_add(used_tkns, MALLOC_CREATE(int, j-1));
					rarray_add(used_tkns, MALLOC_CREATE(int, j+1));
				}
			}
		}
		else if (c == '*' && prev_char == '*' && next_char != '*') {
			for (int j = i + 1; j < line_len; j++) {
				prev_char = ' ';
				prev_prev_char = ' ';
				c = line[j];

				current_tkn = rarray_create(1, sizeof(char *));
				rarray_add(current_tkn, &j);

				if (j > 1)
					prev_char = line[j-1];
				if (tkns_defined(current_tkn, used_tkns)) {
					i++;
					continue;
				}

				if (c == '*' && prev_char == '*') {
					gtk_text_iter_forward_to_end(start_iter_ptr);
					gtk_text_iter_forward_to_end(end_iter_ptr);
					pos = line_len - i;
					gtk_text_iter_backward_chars(start_iter_ptr, pos);
					pos = line_len - j;
					gtk_text_iter_backward_chars(end_iter_ptr, pos);
					gtk_text_buffer_apply_tag(text_buffer, bold_tag, start_iter_ptr, end_iter_ptr);

					deletions[deletions_ptr++] = (text_section){ .start = i - 1, .end = i + 1 };
					deletions[deletions_ptr++] = (text_section){ .start = j - 1, .end = j + 1 };

					rarray_add(used_tkns, MALLOC_CREATE(int, i));
					rarray_add(used_tkns, MALLOC_CREATE(int, i-1));
					rarray_add(used_tkns, MALLOC_CREATE(int, j));
					rarray_add(used_tkns, MALLOC_CREATE(int, j-1));
				}
			}
		}
		else if (c == '*' && prev_char != '*' && next_char != '*') {
			for (int j = i + 1; j < line_len; j++) {
				prev_char = ' ';
				next_char = ' ';
				c = line[j];

				current_tkn = rarray_create(1, sizeof(char *));
				rarray_add(current_tkn, &j);

				if (j > 1)
					prev_char = line[j-1];
				if (j < strlen(line))
					next_char = line[j+1];
				if (tkns_defined(current_tkn, used_tkns)) {
					i++;
					continue;
				}

				if (c == '*' && next_char != '*' && prev_char != '*') {
					gtk_text_iter_forward_to_end(start_iter_ptr);
					gtk_text_iter_forward_to_end(end_iter_ptr);
					pos = line_len - i;
					gtk_text_iter_backward_chars(start_iter_ptr, pos);
					pos = line_len - j;
					gtk_text_iter_backward_chars(end_iter_ptr, pos);
					gtk_text_buffer_apply_tag(text_buffer, italics_tag, start_iter_ptr, end_iter_ptr);

					deletions[deletions_ptr++] = (text_section){ .start = i, .end = i + 1 };
					deletions[deletions_ptr++] = (text_section){ .start = j, .end = j + 1 };

					rarray_add(used_tkns, MALLOC_CREATE(int, i));
					rarray_add(used_tkns, MALLOC_CREATE(int, j));
				}

			}
		}
		else if (c == '[' && !isspace(next_char)) {
			for (int j = i + 1; j < line_len; j++) {
				prev_char = ' ';
				prev_prev_char = ' ';
				next_char = ' ';
				c = line[j];

				current_tkn = rarray_create(1, sizeof(char *));
				rarray_add(current_tkn, &j);

				if (j > 1)
					prev_char = line[j-1];
				if (tkns_defined(current_tkn, used_tkns)) {
					i++;
					continue;
				}

				if (c == '(' && prev_char == ']') {
					for (int k = j + 1; k < line_len; k++) {
						prev_char = ' ';
						c = line[k];

						current_tkn = rarray_create(1, sizeof(char *));
						rarray_add(current_tkn, &k);

						if (k > 1)
							prev_char = line[k-1];
						if (tkns_defined(current_tkn, used_tkns)) {
							i++;
							continue;
						}

						if (c == '(') {
							char *full_url_str = malloc(sizeof(char) * (5 + k - j));
							full_url_str = "url:";
							char *hyperlink = substr(line, j, k - 1);
							strcat(full_url_str, hyperlink);
							GtkTextTag *gtk_tag = create_hyperlink(full_url_str, gtk_text_buffer_get_tag_table(text_buffer));

							gtk_text_iter_forward_to_end(start_iter_ptr);
							gtk_text_iter_forward_to_end(end_iter_ptr);
							pos = line_len - i;
							gtk_text_iter_backward_chars(start_iter_ptr, pos);
							pos = line_len - j + 2;
							gtk_text_iter_backward_chars(end_iter_ptr, pos);
							gtk_text_buffer_apply_tag(text_buffer, gtk_tag, start_iter_ptr, end_iter_ptr);

							deletions[deletions_ptr++] = (text_section){ .start = i - 1, .end = i };
							deletions[deletions_ptr++] = (text_section){ .start = j - 2, .end = k };

							rarray_add(used_tkns, MALLOC_CREATE(int, i));
							rarray_add(used_tkns, MALLOC_CREATE(int, j-1));
							rarray_add(used_tkns, MALLOC_CREATE(int, j));
							rarray_add(used_tkns, MALLOC_CREATE(int, k));
						}
					}
				}
			}
		}

		i++;
	}

	for (int i = 0; i < deletions_ptr; i++) {
		text_section del = deletions[i];
		gtk_text_iter_forward_to_end(start_iter_ptr);
		gtk_text_iter_forward_to_end(end_iter_ptr);
		pos = line_len - del.start;
		gtk_text_iter_backward_chars(start_iter_ptr, pos);
		pos = line_len - del.end;
		gtk_text_iter_backward_chars(end_iter_ptr, pos);
		gtk_text_buffer_delete(text_buffer, start_iter_ptr, end_iter_ptr);
	}
}

void parse(const char *file_content, GtkTextBuffer *text_buffer) {
	int list_indent = 0,
		iteration = 1,
		prev_newline = -1,
		last_line_white = 0,
		c;
	char current_tag_name[3] = "n", last_tag_name[3] = "n";
	char *file_line = NULL;
	GtkTextTag *current_tag;
	GtkTextIter start_iter, end_iter;
	GtkTextIter *start_iter_ptr = &start_iter,
				*end_iter_ptr = &end_iter;

	setup_tagtable(gtk_text_buffer_get_tag_table(text_buffer));

	for (int c = 0; c < strlen(file_content); c++) {
		if (file_content[c] == '\n') {
			file_line = substr(file_content, prev_newline + 1, c);
			prev_newline = c;
		}
		else {
			continue;
		}

		if (file_line == NULL)
			file_line = strdup(" ");

		strcpy(current_tag_name, "n");
		current_tag = normal_tag;

		if (starts_with(file_line, "# ")) {
			printf("H1: %s\n", file_line);
			strcpy(current_tag_name, "h1");
			current_tag = h1_tag;
			char *new_line = substr(file_line, 2, strlen(file_line));
			free(file_line);
			file_line = new_line;
		}
		else if (starts_with(file_line, "## ")) {
			printf("H2: %s\n", file_line);
			strcpy(current_tag_name, "h2");
			current_tag = h2_tag;
			char *new_line = substr(file_line, 3, strlen(file_line));
			free(file_line);
			file_line = new_line;
		}
		else if (starts_with(file_line, "### ")) {
			printf("H3: %s\n", file_line);
			strcpy(current_tag_name, "h3");
			current_tag = h3_tag;
			char *new_line = substr(file_line, 4, strlen(file_line));
			free(file_line);
			file_line = new_line;
		}
		else if (starts_with(trim(file_line), "- ")) {
			printf("LIST ITEM: %s\n", file_line);
			int indent = strchr(file_line, '-') - file_line;
			int old_indent = indent;
			for (int i = 0; i < old_indent; i++)
				if (file_line[i] == '\t')
					indent += 4;

			char *indent_prestr = malloc(sizeof(char) * (indent + 1));
			sprintf(indent_prestr, "%*s", indent, "");
			char *indent_poststr = substr(file_line, old_indent + 1, strlen(file_line));

			int new_line_len = (2 + strlen(indent_prestr) + 2 + strlen(indent_poststr) + 2);
			if (last_line_white != 0) new_line_len += 1;

			char *new_line = malloc(sizeof(char) * new_line_len);
			if (last_line_white != 0) {
				strcpy(new_line, "\n");
				strcat(new_line, indent_prestr);
			}
			else
				strcpy(new_line, indent_prestr);

			strcat(new_line, "• ");
			strcat(new_line, indent_poststr);
			strcat(new_line, "\n");

			free(indent_prestr);
			free(indent_poststr);
			free(file_line);

			file_line = new_line;
			strcpy(current_tag_name, "lt");
		}

		if (list_indent > 0 && strcmp(current_tag_name, "lt") != 0)
			list_indent = 0;

		if (is_whitespace(file_line)) {
			if (last_line_white != 0) {
				last_line_white = last_line_white + 1;
				if (last_line_white % 2 == 0)
					file_line = strdup("\n");
			}
			else {
				last_line_white = 1;
				continue;
			}
		}
		else {
			if ((last_line_white != 0 || (strcmp(current_tag_name, last_tag_name) != 0 && last_line_white % 2 != 0)) && iteration != 1)
				insert_lf(text_buffer);
			last_line_white = 0;
		}

		gtk_text_buffer_get_end_iter(text_buffer, start_iter_ptr);
		GtkTextMark *mark = gtk_text_buffer_create_mark(text_buffer, "mrk", start_iter_ptr, true);

		gtk_text_buffer_insert(text_buffer, start_iter_ptr, file_line, -1);
		gtk_text_buffer_get_iter_at_mark(text_buffer, start_iter_ptr, mark);
		gtk_text_buffer_get_end_iter(text_buffer, end_iter_ptr);
		gtk_text_buffer_apply_tag(text_buffer, current_tag, start_iter_ptr, end_iter_ptr);
		gtk_text_buffer_delete_mark(text_buffer, mark);

		apply_sub_styles(file_line, strlen(file_line), text_buffer);

		iteration = iteration + 1;
		strcpy(last_tag_name, current_tag_name);

		free(file_line);
		file_line = NULL;
	}
}
