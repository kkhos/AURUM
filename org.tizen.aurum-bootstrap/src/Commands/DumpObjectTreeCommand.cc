#include "DumpObjectTreeCommand.h"

#include "ISearchable.h"

#include "Sel.h"
#include "UiDevice.h"
#include "UiObject.h"
#include "UiSelector.h"

#include <loguru.hpp>


DumpObjectTreeCommand::DumpObjectTreeCommand(const ::aurum::ReqDumpObjectTree* request,
                                                   ::aurum::RspDumpObjectTree* response)
    : mRequest{request}, mResponse{response}
{
    mObjMap = ObjectMapper::getInstance();
}

void populateElement(::aurum::Element *el, std::shared_ptr<UiObject> obj)
{
}

void DumpObjectTreeCommand::traverse(::aurum::Element *el, std::string key, int depth)
{
    LOG_SCOPE_F(INFO, "traverse depth:%d el:%p key:%s", depth, el, key.c_str());

    auto obj = mObjMap->getElement(key);
    auto children = obj->getChildren();

    ::aurum::Rect *rect = el->mutable_geometry();
    const Rect<int> &size = obj->getBoundingBox();
    rect->set_x(size.mTopLeft.x);
    rect->set_y(size.mTopLeft.y);
    rect->set_width(size.width());
    rect->set_height(size.height());

    el->set_widget_type(obj->getElementType());
    el->set_widget_style(obj->getElementStyle());
    el->set_value(obj->getText());

    el->set_isshowing(obj->isShowing());
    el->set_isvisible(obj->isVisible());
    el->set_isenabled(obj->isEnabled());
    el->set_isselected(obj->isSelected());
    el->set_ischecked(obj->isChecked());

    for (auto&& child : children) {
        if (!(child->isShowing() && child->isVisible())) continue;
        ::aurum::Element *childEl = el->add_child();
        std::string key2 = mObjMap->addElement(std::move(child));
        childEl->set_elementid(key2);
        traverse(childEl, key2, depth+1);
    }
}

::grpc::Status DumpObjectTreeCommand::execute()
{
    LOG_SCOPE_F(INFO, "DumpObjectTree --------------- ");
    LOG_F(INFO, "elementid : %s", mRequest->elementid().c_str());
    if (mRequest->elementid().length()) {
        ::aurum::Element* root = mResponse->add_roots();
        root->set_elementid(mRequest->elementid());
        traverse(root, mRequest->elementid(), 0);
    } else {
        ;
    }
    return grpc::Status::OK;
}