#include <adwaita.h>
#include "./zip/entries.h"
#include "./folder_button.h"

GtkWidget *create_tree_child(const char *subtitle, bool folder) {
	if (folder) {
		// AdwExpanderRow *child = ADW_EXPANDER_ROW(adw_expander_row_new());
		// adw_expander_row_set_subtitle(child, subtitle);
		//
		// AdwButtonContent *child_content = ADW_BUTTON_CONTENT(adw_button_content_new());
		// adw_button_content_set_icon_name(child_content, "folder-symbolic");
		// adw_expander_row_add_prefix(child, GTK_WIDGET(child_content));
		//
		// return GTK_WIDGET(child);
		FolderButton *child = FOLDER_BUTTON (folder_button_new (subtitle));
		return GTK_WIDGET(child);
	}
	else {
		// AdwActionRow *child = ADW_ACTION_ROW(adw_action_row_new());
		// adw_action_row_set_subtitle(child, subtitle);
		//
		// AdwButtonContent *child_content = ADW_BUTTON_CONTENT(adw_button_content_new());
		// adw_button_content_set_icon_name(child_content, "folder-documents-symbolic");
		// adw_action_row_add_prefix(child, GTK_WIDGET(child_content));
		//
		// GtkButton *child_button = GTK_BUTTON(gtk_button_new());
		// adw_action_row_set_activatable_widget(child, GTK_WIDGET(child_button));
		// adw_action_row_activate(child);
		//
		// return GTK_WIDGET(child);
		return gtk_label_new(subtitle);
	}
}

void add_tree_child(GtkWidget *parent, GtkWidget *child) {
 //    adw_expander_row_add_row(ADW_EXPANDER_ROW(parent), child);
	// gtk_widget_set_margin_start(GTK_WIDGET(child), gtk_widget_get_margin_start(parent) + 10);
	folder_button_add_child(FOLDER_BUTTON(parent), child);
}

GtkWidget *create_tree(NotebookEntry *book) {
	GtkWidget *root = create_tree_child(book->name->val, book->is_folder);
    for (int i = 0; i < book->child_count; ++i) {
		NotebookEntry *child_book = book->children[i];
		GtkWidget *child = create_tree(child_book);
		add_tree_child(root, child);
    }
	return root;
}

