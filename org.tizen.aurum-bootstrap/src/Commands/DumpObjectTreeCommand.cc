#include "bootstrap.h"
#include "DumpObjectTreeCommand.h"
#include "UiObject.h"
#include "UiDevice.h"
#include "UiSelector.h"
#include "Sel.h"
#include "ISearchable.h"

DumpObjectTreeCommand::DumpObjectTreeCommand(const ::aurum::ReqDumpObjectTree *request,
                                                   ::aurum::RspDumpObjectTree *response)
    : mRequest{request}, mResponse{response}
{
    mObjMap = ObjectMapper::getInstance();
}

void DumpObjectTreeCommand::traverse(::aurum::Element *root, std::shared_ptr<Node> node, int depth)
{
    if (!node->mNode) return;
    std::shared_ptr<UiObject> obj = node->mNode;

    obj->refresh();
    if (mObjMap->getElement(obj->getId()) == nullptr)
        mObjMap->addElement(obj);

    root->set_elementid(obj->getId());

    ::aurum::Rect *rect = root->mutable_geometry();
    const Rect<int> &size = obj->getScreenBoundingBox();
    rect->set_x(size.mTopLeft.x);
    rect->set_y(size.mTopLeft.y);
    rect->set_width(size.width());
    rect->set_height(size.height());

    ::aurum::Rect *windowRect = root->mutable_window_relative_geometry();
    const Rect<int> &windowSize = obj->getWindowBoundingBox();
    windowRect->set_x(windowSize.mTopLeft.x);
    windowRect->set_y(windowSize.mTopLeft.y);
    windowRect->set_width(windowSize.width());
    windowRect->set_height(windowSize.height());

    root->set_widget_type(obj->getElementType());
    root->set_widget_style(obj->getElementStyle());

    root->set_text(obj->getText());
    root->set_automationid(obj->getAutomationId());
    root->set_package(obj->getApplicationPackage());
    root->set_role(obj->getRole());

    root->set_ischecked(obj->isChecked());
    root->set_ischeckable(obj->isCheckable());
    root->set_isclickable(obj->isClickable());
    root->set_isenabled(obj->isEnabled());
    root->set_isfocused(obj->isFocused());
    root->set_isfocusable(obj->isFocusable());
    root->set_isscrollable(obj->isScrollable());
    root->set_isselected(obj->isSelected());
    root->set_isshowing(obj->isShowing());
    root->set_isactive(obj->isActive());
    root->set_isvisible(obj->isVisible());
    root->set_isselectable(obj->isSelectable());

    for( auto && childNode : node->mChildren) {
        ::aurum::Element *child = root->add_child();
        traverse(child, childNode, depth+1);
    }
}

::grpc::Status DumpObjectTreeCommand::execute()
{
    LOGI("DumpObjectTree --------------- ");
    LOGI("elementid : %s", mRequest->elementid().c_str());
    if (mRequest->elementid().length()) {
        auto obj = mObjMap->getElement(mRequest->elementid());
        if (!obj) return grpc::Status::OK;;

        auto node = obj->getDescendant();
        ::aurum::Element *root = mResponse->add_roots();
        traverse(root, node, 0);
    }
    return grpc::Status::OK;
}
