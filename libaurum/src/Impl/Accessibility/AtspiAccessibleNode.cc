/*
 * Copyright (c) 2022 Samsung Electronics Co., Ltd All Rights Reserved
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

#include "Aurum.h"

#include "AtspiAccessibleNode.h"
#include "AtspiWrapper.h"

#include <gio/gio.h>

using namespace Aurum;
using namespace AurumInternal;

AtspiAccessibleNode::AtspiAccessibleNode(AtspiAccessible *node)
: mNode{node}
{
    const auto trickDontRemove = std::shared_ptr<AtspiAccessibleNode>( this, [](AtspiAccessibleNode *){} );
    auto watcher = AccessibleWatcher::getInstance();
    watcher->attach(shared_from_this());

    if (mNode) {
        this->updateUniqueId();
        this->updateStates();
    } else {
        LOGI("AtspiAccessibleNode Ctor : mNode is null");
    }
}

AtspiAccessibleNode::~AtspiAccessibleNode()
{
    auto watcher = AccessibleWatcher::getInstance();
    watcher->detach(shared_from_this());
    if(mNode) g_object_unref(mNode);
}

int AtspiAccessibleNode::getChildCount() const
{
    if (!isValid()) {
        return 0;
    }
    int count = AtspiWrapper::Atspi_accessible_get_child_count(mNode, NULL);
    if (count <= 0) return 0;
    return count;
}

std::shared_ptr<AccessibleNode> AtspiAccessibleNode::getChildAt(int index) const
{
    if (!isValid()) {
        return std::make_shared<AtspiAccessibleNode>(nullptr);
    }
    AtspiAccessible *rawChild = AtspiWrapper::Atspi_accessible_get_child_at_index(mNode, index, NULL);
    return std::make_shared<AtspiAccessibleNode>(rawChild);
}

std::vector<std::shared_ptr<AccessibleNode>> AtspiAccessibleNode::getChildren() const
{
    std::vector<std::shared_ptr<AccessibleNode>> ret{};

    GArray *children = AtspiWrapper::Atspi_accessible_get_children(mNode, NULL);
    if (children) {
        ret.reserve(children->len);
        AtspiAccessible *child = nullptr;
        for (unsigned int i = 0; i < children->len; i++) {
            child = g_array_index(children, AtspiAccessible *, i);
            if (child) {
                ret.push_back(std::make_shared<AtspiAccessibleNode>(child));
            }
        }
        g_array_free(children, true);
    }

    return ret;
}

std::shared_ptr<AccessibleNode> AtspiAccessibleNode::getParent() const
{
    if (!isValid()) {
        return std::make_shared<AtspiAccessibleNode>(nullptr);
    }
    AtspiAccessible *rawParent = AtspiWrapper::Atspi_accessible_get_parent(mNode, NULL);
    return std::make_shared<AtspiAccessibleNode>(rawParent);
}

bool AtspiAccessibleNode::isValid() const
{
    if(!AccessibleNode::isValid())  return false;

    AtspiStateSet *st = AtspiWrapper::Atspi_accessible_get_state_set(mNode);
    if (!st) return false;
    if (AtspiWrapper::Atspi_state_set_contains(st, ATSPI_STATE_INVALID) ||
        AtspiWrapper::Atspi_state_set_contains(st, ATSPI_STATE_DEFUNCT)) {
        g_object_unref(st);
        return false;
    }
    g_object_unref(st);
    return true;
}

void* AtspiAccessibleNode::getRawHandler(void) const
{
    return static_cast<void *>(mNode);
}

void AtspiAccessibleNode::updateRoleName()
{
    if (!mRole.empty()) return;

    AtspiWrapper::Atspi_accessible_clear_cache(mNode);

    gchar *rolename = AtspiWrapper::Atspi_accessible_get_role_name(mNode, NULL);
    if (rolename) {
        mRole = rolename;
        g_free(rolename);
    }
}

void AtspiAccessibleNode::updateUniqueId()
{
    if (!mId.empty()) return;

    AtspiWrapper::Atspi_accessible_clear_cache(mNode);

    #ifdef TIZEN
    gchar *uID = AtspiWrapper::Atspi_accessible_get_unique_id(mNode, NULL);
    if (uID) {
        mId = uID;
        g_free(uID);
    }
    #else
        mId = std::string{"N/A"};
    #endif
}

void AtspiAccessibleNode::updateName()
{
    AtspiWrapper::Atspi_accessible_clear_cache(mNode);

    gchar *name = AtspiWrapper::Atspi_accessible_get_name(mNode, NULL);
    if (name) {
        mText = name;
        g_free(name);
    }
}

void AtspiAccessibleNode::updateToolkitName()
{
    if (!mToolkitName.empty()) return;

    AtspiAccessible *app = AtspiWrapper::Atspi_accessible_get_application(mNode, NULL);
    if (app) {
        gchar *toolkitName = AtspiWrapper::Atspi_accessible_get_toolkit_name(app, NULL);
        if (toolkitName) {
            mToolkitName = toolkitName;
            g_free(toolkitName);
        }
        g_object_unref(app);
    }
}

void AtspiAccessibleNode::updateApplication()
{
    if (!mPkg.empty()) return;

    AtspiWrapper::Atspi_accessible_clear_cache(mNode);

    AtspiAccessible *app = AtspiWrapper::Atspi_accessible_get_application(mNode, NULL);
    if (app) {
        gchar *pkg = AtspiWrapper::Atspi_accessible_get_name(app, NULL);
        if (pkg) {
            mPkg = pkg;
            g_free(pkg);
        }
        g_object_unref(app);
    }
}

void AtspiAccessibleNode::updateAttributes()
{
    if (!mType.empty()) return;

    AtspiWrapper::Atspi_accessible_clear_cache(mNode);

    GHashTable *attributes = AtspiWrapper::Atspi_accessible_get_attributes(mNode, NULL);
    if (attributes) {
        char *t = (char*)g_hash_table_lookup(attributes, "type");
        if (!t) t = (char*)g_hash_table_lookup(attributes, "t");
        if (!t) t = (char*)g_hash_table_lookup(attributes, "class");
        char *s = (char*)g_hash_table_lookup(attributes, "style");
        char *a = (char*)g_hash_table_lookup(attributes, "automationId");

        if (t) mType =  std::string(t);
        else {
            if (mRole.empty())updateRoleName();
            mType = mRole;
        }
        if (s) mStyle = std::string(s);
        if (a) mAutomationId = std::string(a);

        g_hash_table_unref(attributes);
    }
}

void AtspiAccessibleNode::updateStates()
{
    resetFeatureProperty();

    AtspiWrapper::Atspi_accessible_clear_cache(mNode);
    AtspiStateSet *st = AtspiWrapper::Atspi_accessible_get_state_set(mNode);
    if (st) {
        GArray *states = AtspiWrapper::Atspi_state_set_get_states(st);
        if (states) {
            AtspiStateType stat;
            for (unsigned int i = 0; states && (i < states->len); ++i) {
                stat = g_array_index(states, AtspiStateType, i);
                setFeatureProperty(stat);
            }
            g_array_free(states, 1);
        }
        g_object_unref(st);
    }
}

void AtspiAccessibleNode::updateExtents()
{
    AtspiComponent *component = AtspiWrapper::Atspi_accessible_get_component_iface(mNode);
    if (component) {
        AtspiRect *screenExtent = AtspiWrapper::Atspi_component_get_extents(
            component, ATSPI_COORD_TYPE_SCREEN, NULL);
        if (screenExtent) {
            mScreenBoundingBox =
                Rect<int>{screenExtent->x, screenExtent->y, screenExtent->x + screenExtent->width,
                        screenExtent->y + screenExtent->height};\
            g_free(screenExtent);
        }

        AtspiRect *windowExtent = AtspiWrapper::Atspi_component_get_extents(
            component, ATSPI_COORD_TYPE_WINDOW, NULL);
        if (windowExtent) {
            mWindowBoundingBox =
                Rect<int>{windowExtent->x, windowExtent->y, windowExtent->x + windowExtent->width,
                        windowExtent->y + windowExtent->height};\
            g_free(windowExtent);
        }
        g_object_unref(component);
    }
}

void AtspiAccessibleNode::updateXPath()
{
    auto XMLDoc = AccessibleWatcher::getInstance()->getXMLDoc(mPkg);
    if (XMLDoc.get() == nullptr) return;

    mXPath = XMLDoc->getXPath(shared_from_this());
}

void AtspiAccessibleNode::updateValue()
{
    AtspiWrapper::Atspi_accessible_clear_cache(mNode);

    AtspiValue *value = AtspiWrapper::Atspi_accessible_get_value(mNode);
    if (value) {
        mMinValue= AtspiWrapper::Atspi_value_get_minimum_value(value, NULL);
        mMaxValue= AtspiWrapper::Atspi_value_get_maximum_value(value, NULL);
        mValue= AtspiWrapper::Atspi_value_get_current_value(value, NULL);
        mIncrement= AtspiWrapper::Atspi_value_get_minimum_increment(value, NULL);
        g_object_unref(value);
    }
}

void AtspiAccessibleNode::updatePid()
{
    if (mPid > 0) return;

    mPid = AtspiWrapper::Atspi_accessible_get_process_id(mNode, NULL);
}

void AtspiAccessibleNode::updateTextMinBoundingRect()
{
    AtspiText *text = atspi_accessible_get_text_iface(mNode);
    if (text)
    {
        gint cc = atspi_text_get_character_count(text, NULL);
        AtspiRect *textMinBoundingRectExtent = AtspiWrapper::Atspi_text_get_minimum_bounding_rectangles(text, 0, cc, ATSPI_COORD_TYPE_WINDOW, NULL);

        if (textMinBoundingRectExtent) {
            mTextMinBoundingRect =
                Rect<int>{textMinBoundingRectExtent->x, textMinBoundingRectExtent->y, textMinBoundingRectExtent->x + textMinBoundingRectExtent->width,
                    textMinBoundingRectExtent->y + textMinBoundingRectExtent->height};
            g_free(textMinBoundingRectExtent);
        }

        g_object_unref(text);
    }
}

void AtspiAccessibleNode::updateInterface()
{
    GArray *interfaces = AtspiWrapper::Atspi_accessible_get_interfaces(mNode);
    if (interfaces)
    {
        for (unsigned int i = 0; i < interfaces->len; i++)
        {
            gchar *interface = g_array_index(interfaces, gchar *, i);
            if (g_strcmp0(interface, "EditableText") == 0 || g_strcmp0(interface, "Value") == 0)
            {
                mInterface = interface;
            }
        }
        g_array_free(interfaces, true);
    }
}

bool AtspiAccessibleNode::setFocus()
{
    AtspiComponent *component = AtspiWrapper::Atspi_accessible_get_component_iface(mNode);
    if (component) {
        bool ret = AtspiWrapper::Atspi_component_grab_focus(component, NULL);
        g_object_unref(component);
        return ret;
    }
    else
        return false;
}

void AtspiAccessibleNode::refresh(bool updateAll)
{
    AtspiWrapper::Atspi_accessible_clear_cache(mNode);

    if (isValid()) {
        if (mRole.empty()) {
            gchar *rolename = AtspiWrapper::Atspi_accessible_get_role_name(mNode, NULL);
            if (rolename) {
                mRole = rolename;
                g_free(rolename);
            }
        }
    #ifdef TIZEN
        if (mId.empty()) {
            gchar *uID = AtspiWrapper::Atspi_accessible_get_unique_id(mNode, NULL);
            if (uID) {
                mId = uID;
                g_free(uID);
            }
        }
    #else
        mId = std::string{"N/A"};
    #endif
        gchar *name = AtspiWrapper::Atspi_accessible_get_name(mNode, NULL);
        if (name) {
            mText = name;
            g_free(name);
        }

        if (mToolkitName.empty()) {
            gchar *toolkitName = AtspiWrapper::Atspi_accessible_get_toolkit_name(mNode, NULL);
            if (toolkitName) {
                mToolkitName = toolkitName;
                g_free(toolkitName);
            }
        }

        if (mPkg.empty()) {
            AtspiAccessible *app = AtspiWrapper::Atspi_accessible_get_application(mNode, NULL);
            if (app) {
                gchar *pkg = AtspiWrapper::Atspi_accessible_get_name(app, NULL);
                if (pkg) {
                    mPkg = pkg;
                    g_free(pkg);
                }
                g_object_unref(app);
            }
        }

        if (mType.empty()) {
            GHashTable *attributes = AtspiWrapper::Atspi_accessible_get_attributes(mNode, NULL);
            if (attributes) {
                char *t = (char*)g_hash_table_lookup(attributes, "type");
                if (!t) t = (char*)g_hash_table_lookup(attributes, "t");
                if (!t) t = (char*)g_hash_table_lookup(attributes, "class");
                char *s = (char*)g_hash_table_lookup(attributes, "style");
                char *a = (char*)g_hash_table_lookup(attributes, "automationId");

                if (t) mType =  std::string(t);
                else mType = mRole;
                if (s) mStyle = std::string(s);
                if (a) mAutomationId = std::string(a);

                g_hash_table_unref(attributes);
            }
        }

        AtspiStateSet *st = AtspiWrapper::Atspi_accessible_get_state_set(mNode);
        if (st) {
            GArray *states = AtspiWrapper::Atspi_state_set_get_states(st);
            if (states) {
                AtspiStateType stat;
                for (unsigned int i = 0; states && (i < states->len); ++i) {
                    stat = g_array_index(states, AtspiStateType, i);
                    setFeatureProperty(stat);
                }
                g_array_free(states, 1);
            }
            g_object_unref(st);
        }
        AtspiComponent *component = AtspiWrapper::Atspi_accessible_get_component_iface(mNode);
        if (component) {
            AtspiRect *screenExtent = AtspiWrapper::Atspi_component_get_extents(
                component, ATSPI_COORD_TYPE_SCREEN, NULL);
            if (screenExtent) {
                mScreenBoundingBox =
                    Rect<int>{screenExtent->x, screenExtent->y, screenExtent->x + screenExtent->width,
                            screenExtent->y + screenExtent->height};\
                g_free(screenExtent);
            }

            AtspiRect *windowExtent = AtspiWrapper::Atspi_component_get_extents(
                component, ATSPI_COORD_TYPE_WINDOW, NULL);
            if (windowExtent) {
                mWindowBoundingBox =
                    Rect<int>{windowExtent->x, windowExtent->y, windowExtent->x + windowExtent->width,
                            windowExtent->y + windowExtent->height};\
                g_free(windowExtent);
            }
            g_object_unref(component);
        }

        AtspiValue *value = AtspiWrapper::Atspi_accessible_get_value(mNode);
        if (value) {
            mMinValue= AtspiWrapper::Atspi_value_get_minimum_value(value, NULL);
            mMaxValue= AtspiWrapper::Atspi_value_get_maximum_value(value, NULL);
            mValue= AtspiWrapper::Atspi_value_get_current_value(value, NULL);
            mIncrement= AtspiWrapper::Atspi_value_get_minimum_increment(value, NULL);
            g_object_unref(value);
        }

        GArray *interfaces = AtspiWrapper::Atspi_accessible_get_interfaces(mNode);
        if (interfaces)
        {
            for (unsigned int i = 0; i < interfaces->len; i++)
            {
                gchar *interface = g_array_index(interfaces, gchar *, i);
                if (g_strcmp0(interface, "EditableText") == 0 || g_strcmp0(interface, "Value") == 0)
                {
                    mInterface = interface;
                }
            }
            g_array_free(interfaces, true);
        }

        if (updateAll) updateXPath();

    } else {
        setFeatureProperty(ATSPI_STATE_INVALID);
    }
}

std::vector<std::string> AtspiAccessibleNode::getActions() const
{
    std::vector<std::string> result{};
    AtspiAction *action;
    if (!isValid()) {
        return result;
    }

    action = AtspiWrapper::Atspi_accessible_get_action_iface(mNode);
    if (action) {
        int a;
        int n_actions = AtspiWrapper::Atspi_action_get_n_actions(action, NULL);

        for (a = 0; a < n_actions; a++) {
            char *action_name = AtspiWrapper::Atspi_action_get_action_name(action, a, NULL);
            if (!action_name) continue;
            result.push_back(std::string{action_name});
            g_free(action_name);
        }
        g_object_unref(action);
    }

    return result;
}

bool AtspiAccessibleNode::doAction(std::string actionName)
{
    AtspiAction *action;

    if (!isValid()) {
        return false;
    }

    action = AtspiWrapper::Atspi_accessible_get_action_iface(mNode);
    if (action) {
        int a;
        int n_actions = AtspiWrapper::Atspi_action_get_n_actions(action, NULL);

        for (a = 0; a < n_actions; a++) {
            char *action_name = AtspiWrapper::Atspi_action_get_action_name(action, a, NULL);
            if (!action_name) {
                 return false;
            }

            if (!strcmp(actionName.c_str(), action_name)) {
                AtspiWrapper::Atspi_action_do_action(action, a, NULL);
                g_free(action_name);
                g_object_unref(action);
                return true;
            }
            g_free(action_name);
        }
        g_object_unref(action);
    }
    return false;
}

bool AtspiAccessibleNode::setValue(std::string text)
{
    if (!isValid()){
        return false;
    }

    AtspiEditableText *iface = AtspiWrapper::Atspi_accessible_get_editable_text(mNode);
    LOGI("set Value iface:%p obj:%p text:%s", iface, mNode, text.c_str() );

    if (!iface) return false;

    updateName();
    int len = getText().length();
    AtspiWrapper::Atspi_editable_text_delete_text(iface, 0, len, NULL);
    bool ret = AtspiWrapper::Atspi_editable_text_insert_text(iface, 0, text.c_str(), text.length(),
                                                             NULL);
    g_object_unref(iface);

    return ret;
}

bool AtspiAccessibleNode::setValue(double value)
{
    if (!isValid()){
        return false;
    }

    AtspiValue *iface = AtspiWrapper::Atspi_accessible_get_value(mNode);
    LOGI("set Value iface:%p obj:%p value:%lf",iface, mNode, value);

    if (!iface) return false;

    updateValue();
    AtspiWrapper::Atspi_value_set_current_value(iface, value, NULL);
    g_object_unref(iface);

    return true;
}

void AtspiAccessibleNode::setFeatureProperty(AtspiStateType type)
{
    switch(type) {
        case ATSPI_STATE_CHECKED:
            setFeatureProperty(NodeFeatureProperties::CHECKED, true);
            break;
        case ATSPI_STATE_CHECKABLE:
            setFeatureProperty(NodeFeatureProperties::CHECKABLE, true);
            break;
        case ATSPI_STATE_ENABLED:
            setFeatureProperty(NodeFeatureProperties::ENABLED, true);
            break;
        case ATSPI_STATE_FOCUSABLE:
            setFeatureProperty(NodeFeatureProperties::FOCUSABLE, true);
            break;
        case ATSPI_STATE_FOCUSED:
            setFeatureProperty(NodeFeatureProperties::FOCUSED, true);
            break;
        case ATSPI_STATE_SELECTABLE:
            setFeatureProperty(NodeFeatureProperties::SELECTABLE, true);
            break;
        case ATSPI_STATE_SELECTED:
            setFeatureProperty(NodeFeatureProperties::SELECTED, true);
            break;
        case ATSPI_STATE_SHOWING:
            setFeatureProperty(NodeFeatureProperties::SHOWING, true);
            break;
        case ATSPI_STATE_VISIBLE:
            setFeatureProperty(NodeFeatureProperties::VISIBLE, true);
            break;
        case ATSPI_STATE_ACTIVE:
            setFeatureProperty(NodeFeatureProperties::ACTIVE, true);
            break;
        case ATSPI_STATE_SENSITIVE:
            setFeatureProperty(NodeFeatureProperties::CLICKABLE, true);
            break;
        case ATSPI_STATE_HIGHLIGHTABLE:
            setFeatureProperty(NodeFeatureProperties::HIGHLIGHTABLE, true);
            break;
        case ATSPI_STATE_DEFUNCT:
        case ATSPI_STATE_INVALID:
            setFeatureProperty(NodeFeatureProperties::INVALID, true);
            break;
        case ATSPI_STATE_TRANSIENT:
        case ATSPI_STATE_TRUNCATED:
        case ATSPI_STATE_ANIMATED:
        case ATSPI_STATE_ARMED:
        case ATSPI_STATE_BUSY:
        case ATSPI_STATE_COLLAPSED:
        case ATSPI_STATE_EDITABLE:
        case ATSPI_STATE_EXPANDABLE:
        case ATSPI_STATE_EXPANDED:
        case ATSPI_STATE_HAS_TOOLTIP:
        case ATSPI_STATE_HORIZONTAL:
        case ATSPI_STATE_ICONIFIED:
        case ATSPI_STATE_MODAL:
        case ATSPI_STATE_MULTI_LINE:
        case ATSPI_STATE_MULTISELECTABLE:
        case ATSPI_STATE_OPAQUE:
        case ATSPI_STATE_PRESSED:
        case ATSPI_STATE_RESIZABLE:
        case ATSPI_STATE_SINGLE_LINE:
        case ATSPI_STATE_STALE:
        case ATSPI_STATE_VERTICAL:
        case ATSPI_STATE_MANAGES_DESCENDANTS:
        case ATSPI_STATE_INDETERMINATE:
        case ATSPI_STATE_REQUIRED:
        case ATSPI_STATE_INVALID_ENTRY:
        case ATSPI_STATE_SUPPORTS_AUTOCOMPLETION:
        case ATSPI_STATE_SELECTABLE_TEXT:
        case ATSPI_STATE_IS_DEFAULT:
        case ATSPI_STATE_VISITED:
        case ATSPI_STATE_HAS_POPUP:
        case ATSPI_STATE_READ_ONLY:
        case ATSPI_STATE_LAST_DEFINED:
        default:
        break;
    }
}
