/*
 * Copyright (c) 2024 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef _ATSPI_WRAPPER_H_
#define _ATSPI_WRAPPER_H_

#include <atspi/atspi.h>
#include <mutex>

namespace AurumInternal {

/**
 * @internal
 *
 * @class AtspiWrapper
 *
 * @ingroup aurum
 *
 * @brief Class that matches atspi Window Node.
 */
class AtspiWrapper {
public:
    /** Wrapper methods for Atspi APIs **/
    static GArray *Atspi_state_set_get_states(AtspiStateSet *set);
    static GArray *Atspi_accessible_get_interfaces(AtspiAccessible *node);
    static gchar *Atspi_accessible_get_name(AtspiAccessible *node, GError **error);
    static AtspiAccessible *Atspi_get_desktop(int n);
    static int Atspi_accessible_get_child_count(AtspiAccessible *node, GError **error);
    static GArray *Atspi_accessible_get_children(AtspiAccessible *node, GError **error);
    static AtspiAccessible *Atspi_accessible_get_child_at_index(AtspiAccessible *node, int index, GError **error);
    static AtspiAccessible *Atspi_accessible_get_parent(AtspiAccessible *node, GError **error);
    static gchar *Atspi_accessible_get_description(AtspiAccessible *node, GError **error);
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
    static gboolean Atspi_component_grab_focus(AtspiComponent *obj, GError **error);
    static gboolean Atspi_component_grab_highlight(AtspiComponent *obj, GError **error);
    static void Atspi_accessible_set_cache_mask(AtspiAccessible *node, AtspiCache mask);
    static AtspiValue *Atspi_accessible_get_value(AtspiAccessible *node);
    static gdouble Atspi_value_get_minimum_value(AtspiValue *iface, GError **error);
    static gdouble Atspi_value_get_current_value(AtspiValue *iface, GError **error);
    static gdouble Atspi_value_get_maximum_value(AtspiValue *iface, GError **error);
    static gboolean Atspi_value_set_current_value(AtspiValue *iface, gdouble value, GError **error);
    static gdouble Atspi_value_get_minimum_increment(AtspiValue *iface, GError **error);
    static guint Atspi_accessible_get_process_id(AtspiAccessible *node, GError **error);
    static gchar *Atspi_accessible_get_toolkit_name(AtspiAccessible *node, GError **error);
    static AtspiRect *Atspi_text_get_minimum_bounding_rectangles(AtspiText* obj, gint start_offset, gint end_offset, AtspiCoordType type, GError** error);
    static void Atspi_accessible_set_listen_post_render(AtspiAccessible *obj, gboolean enabled, GError **error);
    static AtspiCollection *Atspi_accessible_get_collection_iface(AtspiAccessible *node);
    static GArray *Atspi_collection_get_matches(AtspiCollection *obj, AtspiMatchRule *rule, AtspiCollectionSortOrder sortby, gint count, gboolean traverse, GError **error);
    static GArray *Atspi_collection_get_matches_in_matches(AtspiCollection *obj, AtspiMatchRule *first_rule, AtspiMatchRule *second_rule, AtspiCollectionSortOrder sortby, gint first_count, gint second_count, gboolean traverse, GError **error);
    static AtspiAccessibleNodeInfo *Atspi_accessible_get_node_info(AtspiAccessible *obj, GError **error);
    static void Atspi_accessible_free_node_info(AtspiAccessibleNodeInfo *node_info);
    static AtspiAccessible *Atspi_accessible_get_neighbor(AtspiAccessible *root, AtspiAccessible *start, AtspiNeighborSearchDirection direction, GError **error);
    static AtspiAccessible *Atspi_accessible_get_top_layer(AtspiAccessible *obj, GError **error);

private:
    static std::recursive_mutex mMutex;
};

}

#endif
