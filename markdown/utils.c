#include <adwaita.h>

#include "./setup.h"
#include "../arrays.h"

void insert_lf(GtkTextBuffer *text_buffer) {
	GtkTextIter start_iter, end_iter;

    gtk_text_buffer_get_end_iter(text_buffer, &start_iter);
    GtkTextMark *mark = gtk_text_buffer_create_mark(text_buffer, "mrk", &start_iter, true);
    gtk_text_buffer_insert(text_buffer, &start_iter, "\n", -1);
    gtk_text_buffer_get_iter_at_mark(text_buffer, &start_iter, mark);
    gtk_text_buffer_get_end_iter(text_buffer, &end_iter);
    gtk_text_buffer_apply_tag(text_buffer, normal_tag, &start_iter, &end_iter);
    gtk_text_buffer_delete_mark(text_buffer, mark);
}

bool tkns_defined(rarray *queries, rarray *tkns) {
	int i, q, defined = 0;
	bool res = false;

	for (int i = 0; i < queries->size; i++)
		for (int j = 0; j < tkns->size; j++)
			if (*(int*)tkns->items[j] == *(int*)queries->items[i])
				defined = defined + 1;

	if (defined == queries->size)
		return true;
	return false;
}

void insert_used_tkns(rarray *new_tkns, rarray *tkns) {
	for (int i = 0; i < new_tkns->size; i++)
		rarray_add(tkns, new_tkns->items[i]);
}

GtkTextTag *create_hyperlink(char *url, GtkTextTagTable *tag_table) {
    GtkTextTag *gtk_tag = gtk_text_tag_table_lookup(tag_table, url);
    gtk_tag = gtk_text_tag_new(url);

	GValue *style_val = g_new0(GValue, 1);
	g_value_init(style_val, G_TYPE_STRING);
	g_value_set_string(style_val, "blue");
	g_object_set_property(G_OBJECT(gtk_tag), "foreground", style_val);

	GValue *style_val_2 = g_new0(GValue, 1);
	g_value_init(style_val_2, G_TYPE_INT);
	g_value_set_int(style_val_2, PANGO_UNDERLINE_SINGLE);
	g_object_set_property(G_OBJECT(gtk_tag), "underline", style_val_2);

	gtk_text_tag_table_add(tag_table, gtk_tag);
	return gtk_tag;
}

