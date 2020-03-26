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
ISearchable* FindElementCommand::getSearchableTop(void)
{
    ISearchable* searchableObj = nullptr;
    bool fromObject = mRequest->elementid().empty() == false;

    if (fromObject) searchableObj = mObjMap->getElement(mRequest->elementid());
    if (!searchableObj) searchableObj = UiDevice::getInstance(DeviceType::DEFAULT);

    return searchableObj;
}

std::vector<std::shared_ptr<UiSelector>> FindElementCommand::getSelectors(void)
{
    std::vector<std::shared_ptr<UiSelector>> ret = {};
    ret.push_back(Sel::text(mRequest->textfield()));
    return ret;
}

::grpc::Status FindElementCommand::execute()
{
    LOG_SCOPE_F(INFO, "findElement --------------- ");
    auto searchableObj = getSearchableTop();
    auto selectors     = getSelectors();

    std::vector<std::unique_ptr<UiObject>> founds = {};

    for ( auto sel : selectors ) {
        auto ret = searchableObj->findObjects(sel);
        std::move(std::begin(ret), std::end(ret), std::back_inserter(founds));
    }

    if (founds.size() > 0) {
        for (auto& found : founds) {
            UiObject*   obj = found.get();
            std::string key = mObjMap->addElement(std::move(found));
            LOG_F(INFO, "found object : %s key:%s",
                  obj->getResourceName().c_str(), key.c_str());
            ::aurum::Element* elm = mResponse->add_elements();
            elm->set_elementid(key);
        }
        mResponse->set_status(::aurum::RspStatus::OK);
    }

    return grpc::Status::OK;
}