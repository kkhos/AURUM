#include "AccessibleNode.h"
#include <string.h>
#include <iostream>

#include <loguru.hpp>
#include "config.h"

std::map<AtspiAccessible *, AccessibleNode *> AccessibleNode::mNodeMap{};

AccessibleNode::~AccessibleNode()
{

}

AccessibleNode::AccessibleNode() : AccessibleNode(nullptr)
{
    // No meaning without AtspiAccessbile object
    // prohibited to create this object with this constructor
}

AccessibleNode::AccessibleNode(AtspiAccessible *node)
    : mNode(make_gobj_ref_unique(node)), mBoundingBox{0,0,0,0}, mSupportingIfaces(0), mFeatureProperty(0), mIsAlive(true)
{
    // prohibited to create this object this constructor
    // better to use AccessibleNode::get factory method.

    LOG_SCOPE_F(1, "AccessibleNode constructor %p", mNode.get());
    GArray *ifaces = atspi_accessible_get_interfaces(mNode.get());
    if (ifaces) {
        for (unsigned int i = 0; i < ifaces->len; i++) {
            char *iface = g_array_index(ifaces, char *, i);
            if (!strcmp(iface, "Action"))
                mSupportingIfaces |=
                    static_cast<int>(AccessibleNodeInterface::ACTION);
            else if (!strcmp(iface, "Collection"))
                mSupportingIfaces |=
                    static_cast<int>(AccessibleNodeInterface::COLLECTION);
            else if (!strcmp(iface, "Component"))
                mSupportingIfaces |=
                    static_cast<int>(AccessibleNodeInterface::COMPONENT);
            else if (!strcmp(iface, "Document"))
                mSupportingIfaces |=
                    static_cast<int>(AccessibleNodeInterface::DOCUMENT);
            else if (!strcmp(iface, "EditableText"))
                mSupportingIfaces |=
                    static_cast<int>(AccessibleNodeInterface::EDITABLETEXT);
            else if (!strcmp(iface, "Hypertext"))
                mSupportingIfaces |=
                    static_cast<int>(AccessibleNodeInterface::HYPERTEXT);
            else if (!strcmp(iface, "Image"))
                mSupportingIfaces |=
                    static_cast<int>(AccessibleNodeInterface::IMAGE);
            else if (!strcmp(iface, "Selection"))
                mSupportingIfaces |=
                    static_cast<int>(AccessibleNodeInterface::SELECTION);
            else if (!strcmp(iface, "Text"))
                mSupportingIfaces |=
                    static_cast<int>(AccessibleNodeInterface::TEXT);
            else if (!strcmp(iface, "Value"))
                mSupportingIfaces |=
                    static_cast<int>(AccessibleNodeInterface::VALUE);
            else if (!strcmp(iface, "Accessible"))
                mSupportingIfaces |=
                    static_cast<int>(AccessibleNodeInterface::ACCESSIBLE);
            else if (!strcmp(iface, "Table"))
                mSupportingIfaces |=
                    static_cast<int>(AccessibleNodeInterface::TABLE);
            else if (!strcmp(iface, "TableCell"))
                mSupportingIfaces |=
                    static_cast<int>(AccessibleNodeInterface::TABLECELL);
            else
                LOG_F(WARNING, "Not Supported interface found %s", iface);

            g_free(iface);
        }

        g_array_free(ifaces, FALSE);
    }
    this->refresh();
}

std::unique_ptr<AccessibleNode> AccessibleNode::get(AtspiAccessible *node)
{
    return std::make_unique<AccessibleNode>(node);
}

void AccessibleNode::refresh() const
{
    gchar *rolename = atspi_accessible_get_role_name(mNode.get(), NULL);
    if (rolename) {
        mRole = rolename;
        g_free(rolename);
    }

    gchar *uID = atspi_accessible_get_unique_id(mNode.get(), NULL);
    if (uID) {
        mRes = uID;
        g_free(uID);
    }

    GHashTable *attributes = atspi_accessible_get_attributes(mNode.get(), NULL);
    char *t = (char*)g_hash_table_lookup(attributes, "type");
    char *s = (char*)g_hash_table_lookup(attributes, "style");

    if (t) mType =  std::string(t);
    if (s) mStyle = std::string(s);

    free(t);
    free(s);

    g_hash_table_unref(attributes);

    AtspiStateSet *st = atspi_accessible_get_state_set(mNode.get());
    GArray *states = atspi_state_set_get_states(st);

    char *state_name = NULL;
    AtspiStateType stat;
    for (int i = 0; states && (i < states->len); ++i) {
        stat = g_array_index(states, AtspiStateType, i);
        setFeatureProperty(stat);
    }

    if (states) g_array_free(states, 0);
    g_object_unref(st);
}

int AccessibleNode::getChildCount() const
{
    return atspi_accessible_get_child_count(mNode.get(), NULL);
}

std::unique_ptr<AccessibleNode> AccessibleNode::getChildAt(int index) const
{
    AtspiAccessible *child =
        atspi_accessible_get_child_at_index(mNode.get(), index, NULL);
    if (child) {
        auto node = AccessibleNode::get(child);
        g_object_unref(child);
        return node;
    }
    return AccessibleNode::get(nullptr);
}

std::unique_ptr<AccessibleNode> AccessibleNode::getParent() const
{
    AtspiAccessible *parent = atspi_accessible_get_parent(mNode.get(), NULL);
    auto node = AccessibleNode::get(parent);
    if (parent) g_object_unref(parent);
    return node;
}

void AccessibleNode::print(int d, int m) const
{
    if (m <= 0 || d > m) return;

    int             n = 0;
    this->print(d);
    n = getChildCount();

    for (int i = 0; i < n; i++) {
        auto child = getChildAt(i);
        if (child) child->print(d + 1, m);
    }
}

void AccessibleNode::print(int d) const
{
    char *name = atspi_accessible_get_name(mNode.get(), NULL);
    char *role = atspi_accessible_get_role_name(mNode.get(), NULL);
    LOG_F(INFO, "%s - %p(%s)  /  role:%s, pkg:%s, text:%s",
          std::string(d, ' ').c_str(), mNode.get(), name, role, getPkg().c_str(),
          getText().c_str());
    free(name);
    free(role);
}

bool AccessibleNode::isSupporting(AccessibleNodeInterface thisIface) const
{
    return (mSupportingIfaces & static_cast<int>(thisIface)) != 0;
}

bool AccessibleNode::hasFeatureProperty(NodeFeatureProperties prop) const
{
    return (mFeatureProperty & static_cast<int>(prop)) != 0;
}

void AccessibleNode::setFeatureProperty(AtspiStateType type) const
{
/*
    LONGCLICKABLE = 0X0040,
    SCROLLABLE = 0X0080,
*/
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

        case ATSPI_STATE_TRANSIENT:
        case ATSPI_STATE_TRUNCATED:
        case ATSPI_STATE_ANIMATED:
        case ATSPI_STATE_INVALID:
        case ATSPI_STATE_ARMED:
        case ATSPI_STATE_BUSY:
        case ATSPI_STATE_COLLAPSED:
        case ATSPI_STATE_DEFUNCT:
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
        break;
    }
}

void AccessibleNode::setFeatureProperty(NodeFeatureProperties prop, bool has) const
{
    if (has)
        mFeatureProperty |= static_cast<int>(prop);
    else
        mFeatureProperty &= ~static_cast<int>(prop);
}

std::string AccessibleNode::getDesc() const
{
    return mDesc;
}

std::string AccessibleNode::getText() const
{
    gchar *name = atspi_accessible_get_name(mNode.get(), NULL);
    mText = name;
    mPkg = name;
    g_free(name);

    return mText;
}

std::string AccessibleNode::getPkg() const
{
    return mPkg;
}

std::string AccessibleNode::getRes() const
{
    return mRes;
}

std::string AccessibleNode::getType() const
{
    return mType;
}

std::string AccessibleNode::getStyle() const
{
    return mStyle;
}

Rect<int> AccessibleNode::getBoundingBox() const
{
    AtspiComponent *component = atspi_accessible_get_component_iface(mNode.get());
    if (component) {
        AtspiRect *extent = atspi_component_get_extents(
            component, ATSPI_COORD_TYPE_SCREEN, NULL);
        if (extent) {
            mBoundingBox =
                Rect<int>{extent->x, extent->y, extent->x + extent->width,
                          extent->y + extent->height};
            g_free(extent);
        }
        g_object_unref(component);
    }

    return mBoundingBox;
}

bool AccessibleNode::isCheckable() const
{
    return hasFeatureProperty(NodeFeatureProperties::CHECKABLE);
}

bool AccessibleNode::isChecked() const
{
    return hasFeatureProperty(NodeFeatureProperties::CHECKED);
}

bool AccessibleNode::isClickable() const
{
    return hasFeatureProperty(NodeFeatureProperties::CLICKABLE);
}

bool AccessibleNode::isEnabled() const
{
    return hasFeatureProperty(NodeFeatureProperties::ENABLED);
}

bool AccessibleNode::isFocusable() const
{
    return hasFeatureProperty(NodeFeatureProperties::FOCUSABLE);
}

bool AccessibleNode::isFocused() const
{
    return hasFeatureProperty(NodeFeatureProperties::FOCUSED);
}

bool AccessibleNode::isLongClickable() const
{
    return hasFeatureProperty(NodeFeatureProperties::LONGCLICKABLE);
}

bool AccessibleNode::isScrollable() const
{
    return hasFeatureProperty(NodeFeatureProperties::SCROLLABLE);
}

bool AccessibleNode::isSelectable() const
{
    return hasFeatureProperty(NodeFeatureProperties::SELECTABLE);
}

bool AccessibleNode::isSelected() const
{
    return hasFeatureProperty(NodeFeatureProperties::SELECTED);
}

bool AccessibleNode::isVisible() const
{
    return hasFeatureProperty(NodeFeatureProperties::VISIBLE);
}

bool AccessibleNode::isShowing() const
{
    return hasFeatureProperty(NodeFeatureProperties::SHOWING);
}

bool AccessibleNode::isActive() const
{
    return hasFeatureProperty(NodeFeatureProperties::ACTIVE);
}

AtspiAccessible *AccessibleNode::getAccessible() const
{
    return mNode.get();
}

void AccessibleNode::setValue(std::string text) const
{
    AtspiEditableText *iface = atspi_accessible_get_editable_text(mNode.get());
    LOG_F(INFO,"set Value iface:%p obj:%p text:%s", iface, mNode.get(), text.c_str() );
    if (iface) {
        int len = getText().length();
        atspi_editable_text_delete_text(iface, 0, len, NULL);
        atspi_editable_text_insert_text(iface, 0, text.c_str(), text.length(),
                                        NULL);
    }
}
