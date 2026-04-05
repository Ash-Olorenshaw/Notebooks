#ifndef TREE_VIEW_H
#define TREE_VIEW_H

#include "./zip/entries.h"
#include <adwaita.h>

GtkWidget *create_tree_child(const char *subtitle, bool folder);
void add_tree_child(GtkWidget *parent, GtkWidget *child);
GtkWidget *create_tree(NotebookEntry *book);
void update_render_text();

#endif
