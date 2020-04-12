#include "LongClickCommand.h"
#include <loguru.hpp>

#include <UiObject.h>
#include <UiDevice.h>

LongClickCommand::LongClickCommand(const ::aurum::ReqClick* request,
                                   ::aurum::RspClick*       response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status LongClickCommand::execute()
{
    LOG_SCOPE_F(INFO, "LongClick --------------- ");

    ObjectMapper* mObjMap = ObjectMapper::getInstance();
    ::aurum::ReqClick_RequestType type = mRequest->type();

    if (type == ::aurum::ReqClick_RequestType_ELEMENTID) {
        UiObject* obj = mObjMap->getElement(mRequest->elementid());
        if (obj) {
            obj->longClick(LOGNCLICK_INTERVAL);
            mResponse->set_status(::aurum::RspStatus::OK);
        } else
            mResponse->set_status(::aurum::RspStatus::ERROR);
    } else if (type == ::aurum::ReqClick_RequestType_COORD) {
        UiDevice* obj = UiDevice::getInstance(DeviceType::DEFAULT);
        const ::aurum::Point& point = mRequest->coordination();
        obj->click(point.x(), point.y(), LOGNCLICK_INTERVAL);
        mResponse->set_status(::aurum::RspStatus::OK);
    } else if (type == ::aurum::ReqClick_RequestType_ATSPI) {
        mResponse->set_status(::aurum::RspStatus::ERROR);
    }

    return grpc::Status::OK;
}