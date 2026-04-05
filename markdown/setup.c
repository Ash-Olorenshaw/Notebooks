#include <adwaita.h>

GtkTextTag *h1_tag;
GtkTextTag *h2_tag;
GtkTextTag *h3_tag;
GtkTextTag *italics_tag;
GtkTextTag *bold_tag;
GtkTextTag *normal_tag;

GtkTextTag *create_heading(const char *name, int weight, int size) {
	GtkTextTag *tag = gtk_text_tag_new(name);

	GValue *tag_weight = g_new0(GValue, 1);
	g_value_init(tag_weight, G_TYPE_INT);
	g_value_set_int(tag_weight, weight);
	g_object_set_property(G_OBJECT(tag), "weight", tag_weight);

	GValue *tag_scale = g_new0(GValue, 1);
	g_value_init(tag_scale, G_TYPE_INT);
	g_value_set_int(tag_scale, PANGO_SCALE * size);
	g_object_set_property(G_OBJECT(tag), "size", tag_scale);

	return tag;
}

void setup_tagtable(GtkTextTagTable *table) {
	static bool tags_setup = false;
	if (tags_setup)
		return;

	h1_tag = create_heading("heading-one", PANGO_WEIGHT_BOLD, 30);
	h2_tag = create_heading("heading-two", PANGO_WEIGHT_BOLD, 20);
	h3_tag = create_heading("heading-three", PANGO_WEIGHT_BOLD, 12);

	gtk_text_tag_table_add(table, h1_tag);
	gtk_text_tag_table_add(table, h2_tag);
	gtk_text_tag_table_add(table, h3_tag);

	italics_tag = gtk_text_tag_new("italics");
	GValue *italics_tag_style = g_new0(GValue, 1);
	g_value_init(italics_tag_style, G_TYPE_INT);
	g_value_set_int(italics_tag_style, PANGO_STYLE_ITALIC);
	g_object_set_property(G_OBJECT(italics_tag), "style", italics_tag_style);

	gtk_text_tag_table_add(table, italics_tag);

	bold_tag = gtk_text_tag_new("bold");
	GValue *bold_tag_weight = g_new0(GValue, 1);
	g_value_init(bold_tag_weight, G_TYPE_INT);
	g_value_set_int(bold_tag_weight, PANGO_WEIGHT_BOLD);
	g_object_set_property(G_OBJECT(bold_tag), "weight", bold_tag_weight);

	gtk_text_tag_table_add(table, bold_tag);

	normal_tag = gtk_text_tag_new("normal");
	gtk_text_tag_table_add(table, normal_tag);
	tags_setup = true;
}

