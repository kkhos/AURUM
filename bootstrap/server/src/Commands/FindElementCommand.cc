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

::grpc::Status FindElementCommand::execute()
{
    LOG_SCOPE_F(INFO, "findElement --------------- ");

    bool         fromObject = mRequest->elementid().empty() == false;
    ISearchable* searchableObj = nullptr;

    LOG_SCOPE_F(INFO, "fromObject:%d ei:%s tf:%s", fromObject,
                mRequest->elementid().c_str(), mRequest->textfield().c_str());

    if (fromObject) searchableObj = mObjMap->getElement(mRequest->elementid());

    if (searchableObj == nullptr)
        searchableObj = UiDevice::getInstance(DeviceType::DEFAULT);

    std::unique_ptr<UiSelector> sel = Sel::text(mRequest->textfield());
    sel->type(mRequest->widgettype());

    std::vector<std::unique_ptr<UiObject>> founds =
        searchableObj->findObjects(sel.get());

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