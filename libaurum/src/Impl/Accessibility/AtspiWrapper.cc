/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *               http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#include "AtspiWrapper.h"

using namespace AurumInternal;

std::recursive_mutex AtspiWrapper::mMutex = std::recursive_mutex{};

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
    return atspi_accessible_get_editable_text_iface(node);
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
