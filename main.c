#include <adwaita.h>

#include "zipread.h"
#include "./tree_view.h"

NotebookEntry *book;


// Callback function for when a tree button is clicked
static void toggle_tree(GtkButton* self, gpointer user_data) {
    g_print("Tree clicked.\n");
	AdwOverlaySplitView *sidebar = user_data;
	adw_overlay_split_view_set_show_sidebar(sidebar, !adw_overlay_split_view_get_show_sidebar(sidebar));
}

static void on_tree_button_clicked(AdwActionRow *button, gpointer user_data) {
    g_print("Action row clicked.\n");
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

    GtkWidget *content_label = gtk_label_new("Example Main Content\n\nSelect an item from the sidebar tree.");
    gtk_widget_set_hexpand(GTK_WIDGET(content_label), TRUE);
    gtk_widget_set_valign(GTK_WIDGET(content_label), GTK_ALIGN_START);

	AdwNavigationPage *content_box = adw_navigation_page_new(content_label, "title");

    adw_overlay_split_view_set_content(ADW_OVERLAY_SPLIT_VIEW(split_view), GTK_WIDGET(content_box));
	GtkWidget *label = gtk_label_new("Hello World");


	GtkWidget *sidebar_header_bar = adw_header_bar_new();
	GtkWidget *sidebar_toolbar_view = adw_toolbar_view_new();
	adw_toolbar_view_add_top_bar(ADW_TOOLBAR_VIEW (sidebar_toolbar_view), sidebar_header_bar);
	adw_navigation_page_set_child(sidebar, sidebar_toolbar_view);

	// GtkListBox *tree_view = GTK_LIST_BOX(gtk_list_box_new());
	// gtk_list_box_set_selection_mode(tree_view, GTK_SELECTION_NONE);

	book = read_archive();
	// gtk_list_box_append(tree_view, create_tree(book));

	adw_toolbar_view_set_content(ADW_TOOLBAR_VIEW (sidebar_toolbar_view), create_tree(book));

	GtkWidget *content_header_bar = adw_header_bar_new();
	GtkWidget *content_toolbar_view = adw_toolbar_view_new();
	adw_toolbar_view_add_top_bar(ADW_TOOLBAR_VIEW (content_toolbar_view), content_header_bar);
	adw_navigation_page_set_child(content_box, content_toolbar_view);
    adw_application_window_set_content(ADW_APPLICATION_WINDOW(window), split_view);
	GtkWidget *toggle = gtk_toggle_button_new ();
	gtk_button_set_icon_name(GTK_BUTTON (toggle), "sidebar-show-symbolic");
	gtk_widget_set_tooltip_text(toggle, "Toggle sidebar");
	g_signal_connect(toggle, "clicked", G_CALLBACK(toggle_tree), split_view);
	adw_header_bar_pack_start (ADW_HEADER_BAR (content_header_bar), toggle);

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

