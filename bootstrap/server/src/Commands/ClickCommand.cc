#include "ClickCommand.h"

#include "UiObject.h"

#include <loguru.hpp>

ClickCommand::ClickCommand(const ::aurum::ReqClick* request,
                           ::aurum::RspClick*       response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status ClickCommand::execute()
{
    LOG_SCOPE_F(INFO, "Click --------------- ");

    ObjectMapper*                 mObjMap = ObjectMapper::getInstance();
    ::aurum::ReqClick_RequestType type = mRequest->type();

    if (type == ::aurum::ReqClick_RequestType_ELEMENTID) {
        UiObject* obj = mObjMap->getElement(mRequest->elementid());
        if (obj) {
            obj->click();
            mResponse->set_status(::aurum::RspStatus::OK);
        } else
            mResponse->set_status(::aurum::RspStatus::ERROR);
    } else if (type == ::aurum::ReqClick_RequestType_COORD) {
        mResponse->set_status(::aurum::RspStatus::ERROR);
    } else if (type == ::aurum::ReqClick_RequestType_ATSPI) {
        mResponse->set_status(::aurum::RspStatus::ERROR);
    }

    return grpc::Status::OK;
}
