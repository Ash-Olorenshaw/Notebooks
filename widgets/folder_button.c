#include "./folder_button.h"

struct _FolderButton {
    AdwBin parent_instance;

    GtkWidget *button;
    GtkWidget *revealer;
    GtkWidget *content_box;
    AdwButtonContent *button_label;

    gboolean expanded;
};

G_DEFINE_TYPE(FolderButton, folder_button, ADW_TYPE_BIN)

enum {
    PROP_0,
    PROP_EXPANDED,
    PROP_LABEL,
    N_PROPS
};

static GParamSpec *properties[N_PROPS] = { NULL };

static void update_arrow (FolderButton *self) {
    if (self->expanded)
		adw_button_content_set_icon_name(self->button_label, "pan-down-symbolic");
    else
		adw_button_content_set_icon_name(self->button_label, "pan-end-symbolic");
}

static void on_button_clicked (GtkButton *button, FolderButton *self) {
    self->expanded = !self->expanded;
    gtk_revealer_set_reveal_child (GTK_REVEALER (self->revealer), self->expanded);
    update_arrow (self);
    g_object_notify_by_pspec (G_OBJECT (self), properties[PROP_EXPANDED]);
}

static void folder_button_init (FolderButton *self) {
    self->expanded = FALSE;

    // Main vertical box: button on top, revealer below
    GtkWidget *main_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    adw_bin_set_child (ADW_BIN (self), main_box);
	self->button_label = ADW_BUTTON_CONTENT(adw_button_content_new());
	adw_button_content_set_icon_name(self->button_label, "pan-end-symbolic");
	adw_button_content_set_label(self->button_label, "Folder");
	gtk_widget_set_halign(GTK_WIDGET(self->button_label), GTK_ALIGN_START);

    self->button = gtk_button_new();
    gtk_button_set_has_frame(GTK_BUTTON(self->button), FALSE);
    gtk_button_set_child(GTK_BUTTON(self->button), GTK_WIDGET(self->button_label));
    g_signal_connect(self->button, "clicked", G_CALLBACK(on_button_clicked), self);
    gtk_box_append (GTK_BOX (main_box), self->button);

    // Revealer for animated show/hide of children
    self->revealer = gtk_revealer_new();
    gtk_revealer_set_transition_type(GTK_REVEALER(self->revealer), GTK_REVEALER_TRANSITION_TYPE_SLIDE_DOWN);
    gtk_revealer_set_transition_duration(GTK_REVEALER(self->revealer), 250);
    gtk_box_append(GTK_BOX(main_box), self->revealer);

    // Content box for children (indented)
    self->content_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 4);
    gtk_widget_set_margin_start (self->content_box, 24);
    gtk_widget_set_margin_top (self->content_box, 6);
    gtk_widget_set_margin_bottom (self->content_box, 6);
    gtk_revealer_set_child (GTK_REVEALER (self->revealer), self->content_box);
}

static void folder_button_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    FolderButton *self = FOLDER_BUTTON (object);

    switch (prop_id) {
    case PROP_EXPANDED:
        g_value_set_boolean(value, self->expanded);
        break;
    case PROP_LABEL:
        g_value_set_string(value, adw_button_content_get_label(self->button_label));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void folder_button_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    FolderButton *self = FOLDER_BUTTON(object);

    switch (prop_id) {
		case PROP_EXPANDED:
			folder_button_set_expanded(self, g_value_get_boolean (value));
			break;
		case PROP_LABEL:
			folder_button_set_label(self, g_value_get_string (value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static void folder_button_class_init(FolderButtonClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    object_class->get_property = folder_button_get_property;
    object_class->set_property = folder_button_set_property;

    properties[PROP_EXPANDED] = g_param_spec_boolean(
        "expanded",
        "Expanded",
        "Whether the folder is expanded",
        FALSE,
        G_PARAM_READWRITE | G_PARAM_EXPLICIT_NOTIFY);

    properties[PROP_LABEL] = g_param_spec_string(
        "label",
        "Label",
        "The label text",
        "Folder",
        G_PARAM_READWRITE | G_PARAM_EXPLICIT_NOTIFY);

    g_object_class_install_properties(object_class, N_PROPS, properties);
}

GtkWidget *folder_button_new(const char *label) {
    return g_object_new(FOLDER_TYPE_BUTTON, "label", label, NULL);
}

void folder_button_add_child (FolderButton *self, GtkWidget *child) {
    g_return_if_fail(FOLDER_IS_BUTTON (self));
    g_return_if_fail(GTK_IS_WIDGET (child));

    gtk_box_append(GTK_BOX (self->content_box), child);
}

void folder_button_remove_child(FolderButton *self, GtkWidget *child) {
    g_return_if_fail(FOLDER_IS_BUTTON (self));
    g_return_if_fail(GTK_IS_WIDGET (child));

    gtk_box_remove (GTK_BOX (self->content_box), child);
}

void folder_button_set_label (FolderButton *self, const char *label) {
    g_return_if_fail (FOLDER_IS_BUTTON (self));

    adw_button_content_set_label(self->button_label, label);
    g_object_notify_by_pspec (G_OBJECT (self), properties[PROP_LABEL]);
}

void folder_button_set_expanded (FolderButton *self, gboolean expanded) {
    g_return_if_fail (FOLDER_IS_BUTTON (self));

    if (self->expanded == expanded)
        return;

    self->expanded = expanded;
    gtk_revealer_set_reveal_child (GTK_REVEALER (self->revealer), expanded);
    update_arrow (self);
    g_object_notify_by_pspec (G_OBJECT (self), properties[PROP_EXPANDED]);
}

gboolean folder_button_get_expanded (FolderButton *self) {
    g_return_val_if_fail (FOLDER_IS_BUTTON (self), FALSE);
    return self->expanded;
}

