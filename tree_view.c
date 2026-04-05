#include <adwaita.h>
#include "./zip/entries.h"
#include "./widgets/folder_button.h"
#include "./markdown/parse.h"
#include "./globals.h"
#include "./zipread.h"

void clear_buffer() {
	GtkTextIter start_iter, end_iter;

	gtk_text_buffer_get_end_iter(content_text_buffer, &end_iter);
	gtk_text_buffer_get_start_iter(content_text_buffer, &start_iter);
	gtk_text_buffer_delete(content_text_buffer, &start_iter, &end_iter);

	gtk_text_buffer_get_end_iter(raw_text_buffer, &end_iter);
	gtk_text_buffer_get_start_iter(raw_text_buffer, &start_iter);
	gtk_text_buffer_delete(raw_text_buffer, &start_iter, &end_iter);
}

void update_render_text() {
	GtkTextIter start_iter, end_iter;

	gtk_text_buffer_get_end_iter(raw_text_buffer, &end_iter);
	gtk_text_buffer_get_start_iter(raw_text_buffer, &start_iter);
	char *content = gtk_text_buffer_get_text(raw_text_buffer, &start_iter, &end_iter, true);

	if (content != NULL) {
		clear_buffer();
		parse(content, content_text_buffer);
		gtk_text_buffer_set_text(raw_text_buffer, content, -1);
	}
}

static void on_file_click(GtkButton* self, gpointer user_data) {
	char *file_name = user_data;
	static char *content = NULL;

	char *old_file_content = read_zip_file(current_file);

	if (old_file_content != NULL && content != NULL && strcmp(file_name, current_file) != 0 && strcmp(content, old_file_content) != 0) {
		printf("ALERT:\n");
		AdwDialog *alert = adw_dialog_new();
		adw_dialog_set_title(alert, "Save");
		adw_alert_dialog_format_body(ADW_ALERT_DIALOG(alert), "You have unsaved work");
		adw_alert_dialog_add_responses(ADW_ALERT_DIALOG(alert), "save", "Save", "discard", "Discard", "Cancel", "cancel", NULL);
		adw_alert_dialog_set_default_response(ADW_ALERT_DIALOG(alert), "cancel");
		adw_alert_dialog_set_close_response(ADW_ALERT_DIALOG(alert), "cancel");
	}

	if (content != NULL)
		free(content);
	if (old_file_content != NULL)
		free(old_file_content);

	content = read_zip_file(file_name);
	clear_buffer();

	if (content != NULL) {
		parse(content, content_text_buffer);
		gtk_text_buffer_set_text(raw_text_buffer, content, -1);
		current_file = file_name;
	}
}

GtkWidget *create_tree_child(NotebookEntry *book) {
	if (book->is_folder) {
		FolderButton *child = FOLDER_BUTTON(folder_button_new(book->name->val));
		return GTK_WIDGET(child);
	}
	else {
		GtkButton *child = GTK_BUTTON(gtk_button_new());
		AdwButtonContent *child_content = ADW_BUTTON_CONTENT(adw_button_content_new());
		g_signal_connect(child, "clicked", G_CALLBACK(on_file_click), book->zip_file_name->val);

		adw_button_content_set_icon_name(child_content, "folder-documents-symbolic");
		adw_button_content_set_label(child_content, book->name->val);
		gtk_widget_set_halign(GTK_WIDGET(child_content), GTK_ALIGN_START);
		gtk_button_set_child(child, GTK_WIDGET(child_content));
		gtk_button_set_has_frame(child, FALSE);


		return GTK_WIDGET(child);
	}
}

void add_tree_child(GtkWidget *parent, GtkWidget *child) {
	folder_button_add_child(FOLDER_BUTTON(parent), child);
}

GtkWidget *create_tree(NotebookEntry *book) {
	GtkWidget *root = create_tree_child(book);
    for (int i = 0; i < book->child_count; ++i) {
		NotebookEntry *child_book = book->children[i];
		GtkWidget *child = create_tree(child_book);
		add_tree_child(root, child);
    }
	return root;
}

