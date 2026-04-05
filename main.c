#include <adwaita.h>

#include "zipread.h"
#include "globals.h"
#include "./tree_view.h"

GtkTextView *rendered_textview;
GtkTextView *raw_textview;
GtkStack *text_views;

static void save_file(GtkButton* self, gpointer user_data) {
	printf("SAVING\n");
	GtkTextIter start_iter, end_iter;
	if (current_file != NULL) {
		printf("SAVING: %s\n", current_file);
		gtk_text_buffer_get_end_iter(raw_text_buffer, &end_iter);
		gtk_text_buffer_get_start_iter(raw_text_buffer, &start_iter);
		char *content = gtk_text_buffer_get_text(raw_text_buffer, &start_iter, &end_iter, true);
		printf("WRITING: %s\n", content);
		printf("WRITE SUCCESS: %d\n" ,write_zip_file(current_file, content));
	}
}

static void toggle_tree(GtkButton* self, gpointer user_data) {
    g_print("Tree clicked.\n");
	AdwOverlaySplitView *sidebar = user_data;
	adw_overlay_split_view_set_show_sidebar(sidebar, !adw_overlay_split_view_get_show_sidebar(sidebar));
}

static void toggle_view(GtkButton* self, gpointer user_data) {
	if (gtk_stack_get_visible_child(text_views) == GTK_WIDGET(rendered_textview)) {
		gtk_button_set_icon_name(self, "view-reveal-symbolic");
		gtk_widget_set_tooltip_text(GTK_WIDGET(self), "Render markdown");
		gtk_stack_set_visible_child(text_views, GTK_WIDGET(raw_textview));
	}
	else {
		update_render_text();
		gtk_button_set_icon_name(self, "document-edit-symbolic");
		gtk_widget_set_tooltip_text(GTK_WIDGET(self), "Edit markdown");
		gtk_stack_set_visible_child(text_views, GTK_WIDGET(rendered_textview));
	}
}

GtkWidget *pad_widget(int padding, GtkWidget *w) {
	gtk_widget_set_margin_end(w, padding);
	gtk_widget_set_margin_start(w, padding);
	gtk_widget_set_margin_top(w, padding);
	gtk_widget_set_margin_bottom(w, padding);
	return w;
}

GtkWidget *margin_box() {
	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	return pad_widget(10, box);
}

AdwHeaderBar *create_header_bar(GtkWidget *split_view, AdwToolbarView *toolbar_view) {
	AdwHeaderBar *header_bar = ADW_HEADER_BAR(adw_header_bar_new());

	GtkWidget *sidebar_toggle = gtk_toggle_button_new ();
	gtk_button_set_icon_name(GTK_BUTTON (sidebar_toggle), "sidebar-show-symbolic");
	gtk_widget_set_tooltip_text(sidebar_toggle, "Toggle sidebar");
	g_signal_connect(sidebar_toggle, "clicked", G_CALLBACK(toggle_tree), split_view);

	GtkWidget *edit = gtk_toggle_button_new ();
	gtk_button_set_icon_name(GTK_BUTTON (edit), "document-edit-symbolic");
	gtk_widget_set_tooltip_text(edit, "Edit markdown");
	g_signal_connect(edit, "clicked", G_CALLBACK(toggle_view), NULL);

	GtkWidget *save_button = gtk_button_new();
	gtk_button_set_icon_name(GTK_BUTTON(save_button), "document-save-symbolic");
	gtk_widget_set_tooltip_text(save_button, "Save");
	g_signal_connect(save_button, "clicked", G_CALLBACK(save_file), NULL);

	adw_header_bar_pack_start (header_bar, sidebar_toggle);
	adw_header_bar_pack_start(header_bar, edit);
	adw_header_bar_pack_start(header_bar, save_button);
	return header_bar;
}

void create_text_views() {
	text_views = GTK_STACK(gtk_stack_new());

	rendered_textview = GTK_TEXT_VIEW(gtk_text_view_new());
	GtkTextTagTable *content_texttag_table = GTK_TEXT_TAG_TABLE(gtk_text_tag_table_new());
	content_text_buffer = gtk_text_buffer_new(content_texttag_table);
	gtk_text_view_set_buffer(rendered_textview, content_text_buffer);
	gtk_text_view_set_editable(rendered_textview, false);
	gtk_text_view_set_cursor_visible(rendered_textview, false);

	raw_textview = GTK_TEXT_VIEW(gtk_text_view_new());
	raw_text_buffer = gtk_text_buffer_new(gtk_text_tag_table_new());
	gtk_text_view_set_buffer(raw_textview, raw_text_buffer);

	gtk_stack_add_child(text_views, GTK_WIDGET(rendered_textview));
	gtk_stack_add_child(text_views, GTK_WIDGET(raw_textview));
	gtk_text_view_set_wrap_mode(rendered_textview, GTK_WRAP_WORD);
	gtk_text_view_set_wrap_mode(raw_textview, GTK_WRAP_WORD);
	gtk_stack_set_visible_child(text_views, GTK_WIDGET(rendered_textview));
}

static void activate(GtkApplication *app) {
    GtkWidget *window = adw_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW (window), "LibAdwaita Tree Sidebar");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 500);

    GtkWidget *split_view = adw_overlay_split_view_new();
	adw_overlay_split_view_get_enable_hide_gesture(ADW_OVERLAY_SPLIT_VIEW(split_view));

    GtkWidget *gtk_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
	AdwNavigationPage *sidebar = adw_navigation_page_new(gtk_box, "sidebar");
	adw_overlay_split_view_set_sidebar(ADW_OVERLAY_SPLIT_VIEW(split_view), GTK_WIDGET(sidebar));

	GtkWidget *sidebar_header_bar = adw_header_bar_new();
	GtkWidget *sidebar_toolbar_view = adw_toolbar_view_new();
	adw_toolbar_view_add_top_bar(ADW_TOOLBAR_VIEW (sidebar_toolbar_view), sidebar_header_bar);
	adw_navigation_page_set_child(sidebar, sidebar_toolbar_view);

	book = read_archive();

	GtkWidget *box = margin_box();
	gtk_box_append(GTK_BOX(box), create_tree(book));
	adw_toolbar_view_set_content(ADW_TOOLBAR_VIEW (sidebar_toolbar_view), box);

	AdwToolbarView *content_toolbar_view = ADW_TOOLBAR_VIEW(adw_toolbar_view_new());
    adw_overlay_split_view_set_content(ADW_OVERLAY_SPLIT_VIEW(split_view), GTK_WIDGET(content_toolbar_view));
	adw_toolbar_view_add_top_bar(content_toolbar_view, GTK_WIDGET(create_header_bar(split_view, content_toolbar_view)));

	create_text_views();
	GtkScrolledWindow *scrolled_window = GTK_SCROLLED_WINDOW(gtk_scrolled_window_new());
	gtk_scrolled_window_set_child(scrolled_window, GTK_WIDGET(text_views));
	adw_toolbar_view_set_content(content_toolbar_view, GTK_WIDGET(scrolled_window));
    adw_application_window_set_content(ADW_APPLICATION_WINDOW(window), split_view);

    gtk_window_present(GTK_WINDOW(window));
}


int main(int argc, char *argv[]) {
    adw_init();

    AdwApplication *app = adw_application_new("org.example.SimpleTree", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

	cleanup_archive(book);
    return status;
}

