#ifndef MARKDOWN_UTILS_H
#define MARKDOWN_UTILS_H

#include <adwaita.h>
#include "../arrays.h"

void insert_lf(GtkTextBuffer *text_buffer);
bool tkns_defined(rarray *queries, rarray *tkns);
void insert_used_tkns(rarray *new_tkns, rarray *tkns);
GtkTextTag *create_hyperlink(char *url, GtkTextTagTable *tag_table);

#endif
