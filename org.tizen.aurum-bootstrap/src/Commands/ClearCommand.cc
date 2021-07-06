#include <Aurum.h>
#include "ClearCommand.h"
#include <string>

ClearCommand::ClearCommand(const ::aurum::ReqClear* request,
                           ::aurum::RspClear*       response)
    : mRequest{request}, mResponse{response}
{
}

bool ClearCommand::hasHintText(std::shared_ptr<UiObject> obj)
{
    if (!obj) return false;

    auto old_text = obj->getText();
    obj->setText("");
    if (!old_text.compare(obj->getText())) {
        return true;
    } else {
        obj->setText(old_text);
        return false;
    }
}

::grpc::Status ClearCommand::execute()
{
    LOGI("Clear --------------- ");
    ObjectMapper* mObjMap = ObjectMapper::getInstance();
    std::shared_ptr<UiObject> obj = mObjMap->getElement(mRequest->elementid());

    if (obj) {
        obj->setText("");
        obj->refresh();
        auto text = obj->getText();
        if (text.length() != 0) {
            if (hasHintText(obj)) {
                mResponse->set_status(::aurum::RspStatus::OK);
            } else {
                mResponse->set_status(::aurum::RspStatus::ERROR);
            }
        } else {
            mResponse->set_status(::aurum::RspStatus::OK);
        }
    }

    return grpc::Status::OK;
}