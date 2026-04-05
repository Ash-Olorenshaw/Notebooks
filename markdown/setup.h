#ifndef MARKDOWN_SETUP_H
#define MARKDOWN_SETUP_H

#include <adwaita.h>

extern GtkTextTag *h1_tag;
extern GtkTextTag *h2_tag;
extern GtkTextTag *h3_tag;
extern GtkTextTag *italics_tag;
extern GtkTextTag *bold_tag;
extern GtkTextTag *normal_tag;

void setup_tagtable(GtkTextTagTable *table);

#endif
