#ifndef FOLDER_BUTTON_H
#define FOLDER_BUTTON_H

#include <adwaita.h>

G_BEGIN_DECLS

#define FOLDER_TYPE_BUTTON (folder_button_get_type())
G_DECLARE_FINAL_TYPE (FolderButton, folder_button, FOLDER, BUTTON, AdwBin)

GtkWidget *folder_button_new (const char *label);
void       folder_button_add_child    (FolderButton *self, GtkWidget *child);
void       folder_button_remove_child (FolderButton *self, GtkWidget *child);
void       folder_button_set_label    (FolderButton *self, const char *label);
void       folder_button_set_expanded (FolderButton *self, gboolean expanded);
gboolean   folder_button_get_expanded (FolderButton *self);

G_END_DECLS

#endif

