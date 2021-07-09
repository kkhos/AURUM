#include "AtspiWrapper.h"

std::recursive_mutex AtspiWrapper::mMutex = std::recursive_mutex{};
//std::unique_lock<std::mutex> AtspiWrapper::mLock = std::unique_lock<std::mutex>(mMutex, std::defer_lock);

GArray *AtspiWrapper::Atspi_state_set_get_states(AtspiStateSet *set)
{
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    return atspi_state_set_get_states(set);
}

GArray *AtspiWrapper::Atspi_accessible_get_interfaces(AtspiAccessible *node)
{
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    return atspi_accessible_get_interfaces(node);
}

gchar *AtspiWrapper::Atspi_accessible_get_name(AtspiAccessible *node, GError **error)
{
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    return atspi_accessible_get_name(node, error);
}

AtspiAccessible *AtspiWrapper::Atspi_get_desktop(int n)
{
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    return atspi_get_desktop(n);
}

int AtspiWrapper::Atspi_accessible_get_child_count(AtspiAccessible *node, GError **error)
{
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    return atspi_accessible_get_child_count(node, error);
}

AtspiAccessible *AtspiWrapper::Atspi_accessible_get_child_at_index(AtspiAccessible *node, int index, GError **error)
{
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    return atspi_accessible_get_child_at_index(node, index, error);
}

AtspiAccessible *AtspiWrapper::Atspi_accessible_get_parent(AtspiAccessible *node, GError **error)
{
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    return atspi_accessible_get_parent(node, error);
}

AtspiStateSet *AtspiWrapper::Atspi_accessible_get_state_set(AtspiAccessible *node)
{
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    return atspi_accessible_get_state_set(node);
}

gboolean AtspiWrapper::Atspi_state_set_contains(AtspiStateSet *set , AtspiStateType state)
{
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    return atspi_state_set_contains(set , state);
}

gchar *AtspiWrapper::Atspi_accessible_get_role_name(AtspiAccessible *node, GError **error)
{
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    return atspi_accessible_get_role_name(node, error);
}

gchar *AtspiWrapper::Atspi_accessible_get_unique_id(AtspiAccessible *node, GError **error)
{
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    return atspi_accessible_get_unique_id(node, error);
}

GHashTable *AtspiWrapper::Atspi_accessible_get_attributes(AtspiAccessible *node, GError **error)
{
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    return atspi_accessible_get_attributes(node, error);
}

AtspiComponent *AtspiWrapper::Atspi_accessible_get_component_iface(AtspiAccessible *node)
{
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    return atspi_accessible_get_component_iface(node);
}

AtspiRect *AtspiWrapper::Atspi_component_get_extents(AtspiComponent *obj, AtspiCoordType ctype, GError **error)
{
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    return atspi_component_get_extents(obj, ctype, error);
}

AtspiAction *AtspiWrapper::Atspi_accessible_get_action_iface(AtspiAccessible *node)
{
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    return atspi_accessible_get_action_iface(node);
}

int AtspiWrapper::Atspi_action_get_n_actions(AtspiAction *action, GError **error)
{
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    return atspi_action_get_n_actions(action, error);
}

gchar *AtspiWrapper::Atspi_action_get_action_name(AtspiAction *action, int index, GError **error)
{
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    return atspi_action_get_action_name(action, index, error);
}

gboolean AtspiWrapper::Atspi_action_do_action(AtspiAction *action, int index, GError **error)
{
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    return atspi_action_do_action(action, index, error);
}

AtspiEditableText *AtspiWrapper::Atspi_accessible_get_editable_text(AtspiAccessible *node)
{
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    return atspi_accessible_get_editable_text(node);
}

gboolean AtspiWrapper::Atspi_editable_text_delete_text(AtspiEditableText *iface, int start, int end, GError **error)
{
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    return atspi_editable_text_delete_text(iface, start, end, error);
}

gboolean AtspiWrapper::Atspi_editable_text_insert_text(AtspiEditableText *iface, int pos, const gchar *text, int len, GError **error)
{
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    return atspi_editable_text_insert_text(iface, pos, text, len, error);
}

AtspiAccessible *AtspiWrapper::Atspi_accessible_get_application (AtspiAccessible *node, GError **error)
{
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    return atspi_accessible_get_application(node, error);
}

void AtspiWrapper::Atspi_accessible_clear_cache (AtspiAccessible *node)
{
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    return atspi_accessible_clear_cache(node);
}
void AtspiWrapper::lock()
{
    mMutex.lock();
}

void AtspiWrapper::unlock()
{
    mMutex.unlock();
}
