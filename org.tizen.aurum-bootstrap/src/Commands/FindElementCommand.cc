#include "FindElementCommand.h"

#include "ISearchable.h"

#include "Sel.h"
#include "UiDevice.h"
#include "UiObject.h"
#include "UiSelector.h"

#include <loguru.hpp>

FindElementCommand::FindElementCommand(const ::aurum::ReqFindElement* request,
                                       ::aurum::RspFindElement*       response)
    : mRequest{request}, mResponse{response}
{
    mObjMap = ObjectMapper::getInstance();
}
std::shared_ptr<ISearchable> FindElementCommand::getSearchableTop(void)
{
    std::shared_ptr<ISearchable> searchableObj{nullptr};

    if (mRequest->_automationid_case() != 0) {
        searchableObj = mObjMap->getElement(mRequest->elementid());
    }
    if (!searchableObj) searchableObj = UiDevice::getInstance();

    return searchableObj;
}

std::vector<std::shared_ptr<UiSelector>> FindElementCommand::getSelectors(void)
{
    auto sel = std::make_shared<UiSelector>();

    if(mRequest->_automationid_case())    sel->id(mRequest->automationid());
    if(mRequest->_textfield_case())       sel->text(mRequest->textfield());
    if(mRequest->_widgettype_case())      sel->type(mRequest->widgettype());
    if(mRequest->_widgetstyle_case())     sel->style(mRequest->widgetstyle());
    if(mRequest->_ischecked_case())       sel->isChecked(mRequest->ischecked());
    if(mRequest->_ischeckable_case())     sel->isCheckable(mRequest->ischeckable());
    if(mRequest->_isclickable_case())     sel->isClickable(mRequest->isclickable());
    if(mRequest->_isenabled_case())       sel->isEnabled(mRequest->isenabled());
    if(mRequest->_isfocused_case())       sel->isFocused(mRequest->isfocused());
    if(mRequest->_isfocusable_case())     sel->isFocusable(mRequest->isfocusable());
    if(mRequest->_isscrollable_case())    sel->isScrollable(mRequest->isscrollable());
    if(mRequest->_isselected_case())      sel->isSelected(mRequest->isselected());
    if(mRequest->_isshowing_case())       sel->isShowing(mRequest->isshowing());
    if(mRequest->_isactive_case())        sel->isActive(mRequest->isactive());
    if(mRequest->_mindepth_case())        sel->minDepth(mRequest->mindepth());
    if(mRequest->_maxdepth_case())        sel->maxDepth(mRequest->maxdepth());

    return std::vector<std::shared_ptr<UiSelector>>{sel};
}

::grpc::Status FindElementCommand::execute()
{
    LOG_SCOPE_F(INFO, "findElement --------------- ");
    auto searchableObj = getSearchableTop();
    auto selectors     = getSelectors();

    std::vector<std::shared_ptr<UiObject>> founds = {};

    for ( auto &sel : selectors ) {
        auto ret = searchableObj->findObjects(sel);
        std::move(std::begin(ret), std::end(ret), std::back_inserter(founds));
    }

    if (founds.size() > 0) {
        for (auto& found : founds) {
            UiObject*   obj = found.get();
            std::string key{};
            key = mObjMap->getElement(found);
            if (key.length() <= 0)
              key = mObjMap->addElement(std::move(found));
            LOG_F(INFO, "found object : %s key:%s",
                  obj->getAutomationId().c_str(), key.c_str());
            ::aurum::Element* elm = mResponse->add_elements();
            elm->set_elementid(key);
            elm->set_package(obj->getApplicationPackage());

            ::aurum::Rect *rect = elm->mutable_geometry();
            const Rect<int> &size = obj->getBoundingBox();
            rect->set_x(size.mTopLeft.x);
            rect->set_y(size.mTopLeft.y);
            rect->set_width(size.width());
            rect->set_height(size.height());
            elm->set_widget_type(obj->getElementType());
            elm->set_widget_style(obj->getElementStyle());

            elm->set_text(obj->getText());
            elm->set_id(obj->getId());
            elm->set_automationid(obj->getAutomationId());
            elm->set_package(obj->getApplicationPackage());
            elm->set_role(obj->getRole());

            elm->set_ischecked(obj->isChecked());
            elm->set_ischeckable(obj->isCheckable());
            elm->set_isclickable(obj->isClickable());
            elm->set_isenabled(obj->isEnabled());
            elm->set_isfocused(obj->isFocused());
            elm->set_isfocusable(obj->isFocusable());
            elm->set_isscrollable(obj->isScrollable());
            elm->set_isselected(obj->isSelected());
            elm->set_isshowing(obj->isShowing());
            elm->set_isactive(obj->isActive());
            elm->set_isvisible(obj->isVisible());
            elm->set_isselectable(obj->isSelectable());
        }
        mResponse->set_status(::aurum::RspStatus::OK);
    }

    return grpc::Status::OK;
}
