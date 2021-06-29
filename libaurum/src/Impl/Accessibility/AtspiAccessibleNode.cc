#include "AtspiAccessibleNode.h"
#include "AccessibleWatcher.h"
#include "AtspiWrapper.h"

#include <gio/gio.h>

#include <loguru.hpp>

//std::map<AtspiAccessible *, AccessibleNode *> AccessibleNode::mNodeMap{};

AtspiAccessibleNode::AtspiAccessibleNode(AtspiAccessible *node)
: mNode{node}
{
    const auto trickDontRemove = std::shared_ptr<AtspiAccessibleNode>( this, [](AtspiAccessibleNode*){} );
    auto watcher = AccessibleWatcher::getInstance();
    watcher->attach(shared_from_this());

    if (mNode) {
        /*
        GArray *ifaces = AtspiWrapper::Atspi_accessible_get_interfaces(mNode);
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
        }*/

        this->refresh();
    } else {
        LOG_F(INFO, "AtspiAccessibleNode Ctor : mNode is null");
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
    AtspiWrapper::lock();
    if (!isValid()) {
        AtspiWrapper::unlock();
        return 0;
    }
    int count = AtspiWrapper::Atspi_accessible_get_child_count(mNode, NULL);
    AtspiWrapper::unlock();
    if (count <= 0) return 0;
    return count;
}

std::shared_ptr<AccessibleNode> AtspiAccessibleNode::getChildAt(int index) const
{
    AtspiWrapper::lock();
    if (!isValid()) {
        AtspiWrapper::unlock();
        return std::make_shared<AtspiAccessibleNode>(nullptr);
    }
    LOG_SCOPE_F(INFO, "getChild @ %d from node(%p)", index, mNode);
    AtspiAccessible *rawChild = AtspiWrapper::Atspi_accessible_get_child_at_index(mNode, index, NULL);
    AtspiWrapper::unlock();
    return std::make_shared<AtspiAccessibleNode>(rawChild);
}

std::vector<std::shared_ptr<AccessibleNode>> AtspiAccessibleNode::getChildren() const
{
    std::vector<std::shared_ptr<AccessibleNode>> ret{};
    int nchild = this->getChildCount();
    for (int i = 0; i < nchild; i++) {
        auto child = getChildAt(i);
        if (child) ret.push_back(child);
    }
    return ret;
}

std::shared_ptr<AccessibleNode> AtspiAccessibleNode::getParent() const
{
    AtspiWrapper::lock();
    if (!isValid()) {
        AtspiWrapper::unlock();
        return std::make_shared<AtspiAccessibleNode>(nullptr);
    }
    AtspiAccessible *rawParent = AtspiWrapper::Atspi_accessible_get_parent(mNode, NULL);
    AtspiWrapper::unlock();
    return std::make_shared<AtspiAccessibleNode>(rawParent);
/*  auto node = AccessibleNode::get(parent);
    if (parent) g_object_unref(parent);
    return node; */
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
    return static_cast<void*>(mNode);
}

void AtspiAccessibleNode::refresh()
{
    AtspiWrapper::lock();

    AtspiWrapper::Atspi_accessible_clear_cache(mNode);

    if (isValid()) {
        gchar *rolename = AtspiWrapper::Atspi_accessible_get_role_name(mNode, NULL);
        if (rolename) {
            mRole = rolename;
            g_free(rolename);
        }
    #ifdef TIZEN
        gchar *uID = AtspiWrapper::Atspi_accessible_get_unique_id(mNode, NULL);
        if (uID) {
            mId = uID;
            g_free(uID);
        }
    #else
        mId = std::string{"N/A"};
    #endif

        gchar *name = AtspiWrapper::Atspi_accessible_get_name(mNode, NULL);
        if (name) {
            mText = name;
            g_free(name);
        }

        AtspiAccessible *app = AtspiWrapper::Atspi_accessible_get_application(mNode, NULL);
        if (app) {
            gchar *pkg = AtspiWrapper::Atspi_accessible_get_name(app, NULL);
            if (pkg) {
                mPkg = pkg;
                g_free(pkg);
            }
            g_object_unref(app);
        }

        GHashTable *attributes = AtspiWrapper::Atspi_accessible_get_attributes(mNode, NULL);
        if (attributes) {
            char *t = (char*)g_hash_table_lookup(attributes, "type");
            if (!t) t = (char*)g_hash_table_lookup(attributes, "t");
            if (!t) t = (char*)g_hash_table_lookup(attributes, "class");
            char *s = (char*)g_hash_table_lookup(attributes, "style");
            char *a = (char*)g_hash_table_lookup(attributes, "automationId");

            if (t) mType =  std::string(t);
            if (s) mStyle = std::string(s);
            if (a) mAutomationId = std::string(a);

            g_hash_table_unref(attributes);
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
            AtspiRect *extent = AtspiWrapper::Atspi_component_get_extents(
                component, ATSPI_COORD_TYPE_SCREEN, NULL);
            if (extent) {
                mBoundingBox =
                    Rect<int>{extent->x, extent->y, extent->x + extent->width,
                            extent->y + extent->height};
                g_free(extent);
            }
            g_object_unref(component);
        }
    } else {
        setFeatureProperty(ATSPI_STATE_INVALID);
    }
    AtspiWrapper::unlock();
}

std::vector<std::string> AtspiAccessibleNode::getActions() const
{
    AtspiWrapper::lock();

    std::vector<std::string> result{};
    AtspiAction *action;
    if (!isValid()) {
        AtspiWrapper::unlock();
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

    AtspiWrapper::unlock();

    return result;
}

bool AtspiAccessibleNode::doAction(std::string actionName)
{
    AtspiWrapper::lock();
    AtspiAction *action;

    if (!isValid()) {
        AtspiWrapper::unlock();
        return false;
    }

    action = AtspiWrapper::Atspi_accessible_get_action_iface(mNode);
    if (action) {
        int a;
        int n_actions = AtspiWrapper::Atspi_action_get_n_actions(action, NULL);

        for (a = 0; a < n_actions; a++) {
            char *action_name = AtspiWrapper::Atspi_action_get_action_name(action, a, NULL);
            if (!action_name) {
                AtspiWrapper::unlock();
                 return false;
            }

            if (!strcmp(actionName.c_str(), action_name)) {
                AtspiWrapper::Atspi_action_do_action(action, a, NULL);
                g_free(action_name);
                g_object_unref(action);
                AtspiWrapper::unlock();
                return true;
            }
            g_free(action_name);
        }
        g_object_unref(action);
    }
    AtspiWrapper::unlock();
    return false;
}

void AtspiAccessibleNode::setValue(std::string text)
{
    AtspiWrapper::lock();
    if (!isValid()){
        AtspiWrapper::unlock();
        return;
    }

    AtspiEditableText *iface = AtspiWrapper::Atspi_accessible_get_editable_text(mNode);
    LOG_F(INFO,"set Value iface:%p obj:%p text:%s", iface, mNode, text.c_str() );
    if (iface) {
        int len = getText().length();
        AtspiWrapper::Atspi_editable_text_delete_text(iface, 0, len, NULL);
        AtspiWrapper::Atspi_editable_text_insert_text(iface, 0, text.c_str(), text.length(),
                                        NULL);
    }
    AtspiWrapper::unlock();
}

void AtspiAccessibleNode::setFeatureProperty(AtspiStateType type)
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
