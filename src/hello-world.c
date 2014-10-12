#include <shell/e-shell-sidebar.h>
#include <shell/e-shell-view.h>
#include <shell/e-shell-window.h>
#include <mail/em-folder-tree.h>
#include <mail/em-folder-selector.h>
#include <libebackend/libebackend.h>
#include "gcc-diagnostics.h"

typedef struct _EHelloWorld EHelloWorld;
typedef struct _EHelloWorldClass EHelloWorldClass;

struct _EHelloWorld {
        EExtension parent;
        guint ui_merge_id;
};

struct _EHelloWorldClass {
        EExtensionClass parent_class;
};

/* Module Entry Points */
void e_module_load (GTypeModule *type_module);
void e_module_unload (GTypeModule *type_module);

/* Forward Declarations */
GType e_hello_world_get_type (void);

G_DEFINE_DYNAMIC_TYPE (EHelloWorld, e_hello_world, E_TYPE_EXTENSION)
#define E_HELLO_WORLD(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST \
    ((obj), e_hello_world_get_type(), EHelloWorld))

static const gchar *ui =
"<menubar name='main-menu'>"
"  <placeholder name='custom-menus'>"
"    <menu action='mail-folder-menu'>"
"      <menuitem action='go-to-mail-folder'/>"
"    </menu>"
"  </placeholder>"
"</menubar>";

static void
mail_shell_view_created_cb (EShellWindow *, EShellView *, EHelloWorld *);

static void
action_go_to_mail_folder_cb (GtkAction *, EShellWindow *);

static void
mail_shell_view_toggled_cb (EShellView *, EHelloWorld *);

static GtkActionEntry menu_entries[] = {
    {
        "go-to-mail-folder", // name
        "go-jump", // stock_id
        "Go To...", // label
        "<Ctrl>semicolon", // accelerator
        "Open a dialog to go to another mail folder", // tooltip
        G_CALLBACK (action_go_to_mail_folder_cb) // callback
    },
};


static void
e_hello_world_constructed (GObject *object)
{
        /* Chain up to parent's constructed() method. */
        G_OBJECT_CLASS (e_hello_world_parent_class)->constructed (object);

        EHelloWorld *self = E_HELLO_WORLD(object);
        EExtensible *extensible = e_extension_get_extensible (E_EXTENSION (object));

        EShellWindow *window = E_SHELL_WINDOW (extensible);

        EShellView *mail_view = e_shell_window_peek_shell_view(window, "mail");
        if (mail_view != NULL) {
            // Run now if the mail view is already created
            mail_shell_view_created_cb(window, mail_view, self);
        } else {
            // Schedule initialisation for when the mail view is created
            g_signal_connect(window, "shell-view-created::mail", G_CALLBACK(mail_shell_view_created_cb), self);
        }
}

static void
e_hello_world_finalize (GObject *object)
{
        /* Chain up to parent's finalize() method. */
        G_OBJECT_CLASS (e_hello_world_parent_class)->finalize (object);
}

static void
e_hello_world_class_init (EHelloWorldClass *class)
{
        GObjectClass *object_class;
        EExtensionClass *extension_class;

        object_class = G_OBJECT_CLASS (class);
        object_class->constructed = e_hello_world_constructed;
        object_class->finalize = e_hello_world_finalize;

        /* Specify the GType of the class we're extending.
         * The class must implement the EExtensible interface. */
        extension_class = E_EXTENSION_CLASS (class);
        extension_class->extensible_type = E_TYPE_SHELL_WINDOW;
}

static void
e_hello_world_class_finalize (EHelloWorldClass *class)
{
        /* This function is usually left empty. */
}

static void
e_hello_world_init (EHelloWorld *extension)
{
        /* The EShell object we're extending is not available yet,
         * but we could still do some early initialization here. */
}

static void
mail_shell_view_created_cb (EShellWindow *window, EShellView *view, EHelloWorld *self)
{
        g_signal_connect(view, "toggled", G_CALLBACK(mail_shell_view_toggled_cb), self);

        GtkActionGroup *mail_action_group = e_shell_window_get_action_group(window, "mail");
        IGNORING(deprecated-declarations,
            gtk_action_group_add_actions(mail_action_group, menu_entries, G_N_ELEMENTS(menu_entries), window);
        )

        // Ensure the toggled callback runs if the view is already visible
        mail_shell_view_toggled_cb(view, self);
}

static void
mail_shell_view_toggled_cb (EShellView *view, EHelloWorld *self)
{
        EShellWindow *window = e_shell_view_get_shell_window(view);
        GtkUIManager *ui_manager = e_shell_window_get_ui_manager(window);
        gboolean active = e_shell_view_is_active(view);

        if (active && self->ui_merge_id == 0) {
            // Add the menu item the first time the mail view is shown
            GError *error = NULL;
            IGNORING(deprecated-declarations,
                self->ui_merge_id = gtk_ui_manager_add_ui_from_string(ui_manager, ui, -1, &error);
            )
            if (error != NULL) {
                g_warning("Unable to add menu item: %s", error->message);
                g_error_free(error);
            }
        } else if (!active && self->ui_merge_id != 0) {
            // Remove the menu item the first time the mail view is hidden
            IGNORING(deprecated-declarations,
                gtk_ui_manager_remove_ui (ui_manager, self->ui_merge_id);
            )
            self->ui_merge_id = 0;
        }
}

static void
action_go_to_mail_folder_cb (GtkAction *action, EShellWindow *mail_shell_window)
{
        EShellView *mail_shell_view = e_shell_window_get_shell_view (mail_shell_window, "mail");
        EShellSidebar *mail_shell_sidebar = e_shell_view_get_shell_sidebar (mail_shell_view);
        EMFolderTree *mail_folder_tree;
        g_object_get (mail_shell_sidebar, "folder-tree", &mail_folder_tree, NULL);

        EMFolderTreeModel *model = em_folder_tree_model_get_default();

        GtkWidget *dialog = em_folder_selector_new(GTK_WINDOW(mail_shell_window), model);
        gtk_window_set_title(GTK_WINDOW(dialog), "Go To");

        EMFolderSelector *selector = EM_FOLDER_SELECTOR(dialog);
        em_folder_selector_set_default_button_label(selector, "Go");

        // Set the initial selection to the currently-open folder
        EMFolderTree *selector_folder_tree = em_folder_selector_get_folder_tree(selector);
        const gchar *current_folder_uri = em_folder_tree_get_selected_uri(mail_folder_tree);
        em_folder_tree_set_selected(selector_folder_tree, current_folder_uri, FALSE);

        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
            // Select the folder based on its URI
            const gchar *uri = em_folder_selector_get_selected_uri(selector);
            em_folder_tree_set_selected (mail_folder_tree, uri, FALSE);
        }

        gtk_widget_destroy(dialog);
}

G_MODULE_EXPORT void
e_module_load (GTypeModule *type_module)
{
        /* This registers our EShell extension class with the GObject
         * type system.  An instance of our extension class is paired
         * with each instance of the class we're extending. */
        e_hello_world_register_type (type_module);
}

G_MODULE_EXPORT void
e_module_unload (GTypeModule *type_module)
{
        /* This function is usually left empty. */
}