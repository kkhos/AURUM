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
            std::string key = mObjMap->addElement(std::move(found));
            LOG_F(INFO, "found object : %s key:%s",
                  obj->getAutomationId().c_str(), key.c_str());
            ::aurum::Element* elm = mResponse->add_elements();
            elm->set_elementid(key);
        }
        mResponse->set_status(::aurum::RspStatus::OK);
    }

    return grpc::Status::OK;
}