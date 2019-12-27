#include "ClickCommand.h"

#include "UiObject.h"
#include "UiDevice.h"

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
        UiDevice* obj = UiDevice::getInstance(DeviceType::DEFAULT);
        const ::aurum::Point& point = mRequest->coordination();
        obj->click(point.x(), point.y());
        mResponse->set_status(::aurum::RspStatus::OK);
    } else if (type == ::aurum::ReqClick_RequestType_ATSPI) {
        mResponse->set_status(::aurum::RspStatus::ERROR);
    }

    return grpc::Status::OK;
}
