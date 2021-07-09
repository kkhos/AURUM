#pragma once

#include <atspi/atspi.h>
#include <mutex>

class AtspiWrapper {
public:
    static GArray *Atspi_state_set_get_states(AtspiStateSet *set);
    static GArray *Atspi_accessible_get_interfaces(AtspiAccessible *node);
    static gchar *Atspi_accessible_get_name(AtspiAccessible *node, GError **error);
    static AtspiAccessible *Atspi_get_desktop(int n);
    static int Atspi_accessible_get_child_count(AtspiAccessible *node, GError **error);
    static AtspiAccessible *Atspi_accessible_get_child_at_index(AtspiAccessible *node, int index, GError **error);
    static AtspiAccessible *Atspi_accessible_get_parent(AtspiAccessible *node, GError **error);
    static AtspiStateSet *Atspi_accessible_get_state_set(AtspiAccessible *node);
    static gboolean Atspi_state_set_contains(AtspiStateSet *set , AtspiStateType state);
    static gchar *Atspi_accessible_get_role_name(AtspiAccessible *node, GError **error);
    static gchar *Atspi_accessible_get_unique_id(AtspiAccessible *node, GError **error);
    static GHashTable *Atspi_accessible_get_attributes(AtspiAccessible *node, GError **error);
    static AtspiComponent *Atspi_accessible_get_component_iface(AtspiAccessible *node);
    static AtspiRect *Atspi_component_get_extents(AtspiComponent *obj, AtspiCoordType ctype, GError **error);
    static AtspiAction *Atspi_accessible_get_action_iface(AtspiAccessible *node);
    static int Atspi_action_get_n_actions(AtspiAction *action, GError **error);
    static gchar *Atspi_action_get_action_name(AtspiAction *action, int index, GError **error);
    static gboolean Atspi_action_do_action(AtspiAction *action, int index, GError **error);
    static AtspiEditableText *Atspi_accessible_get_editable_text(AtspiAccessible *node);
    static gboolean Atspi_editable_text_delete_text(AtspiEditableText *, int start, int end, GError **error);
    static gboolean Atspi_editable_text_insert_text(AtspiEditableText *, int pos, const gchar *text, int len, GError **error);
    static AtspiAccessible *Atspi_accessible_get_application (AtspiAccessible *node, GError **error);
    static void Atspi_accessible_clear_cache (AtspiAccessible *node);

    static void lock();
    static void unlock();
private:
    static std::recursive_mutex mMutex;
    //static std::unique_lock<std::mutex> mLock;
};
