#include "FlickCommand.h"
#include <loguru.hpp>

#include <UiDevice.h>

FlickCommand::FlickCommand(const ::aurum::ReqFlick *request,
                           ::aurum::RspFlick *      response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status FlickCommand::execute()
{
    LOG_SCOPE_F(INFO, "Flick --------------- ");
    // ObjectMapper *mObjMap = ObjectMapper::getInstance();

    const ::aurum::Point &startPoint = mRequest->startpoint();
    const ::aurum::Point &endPoint = mRequest->endpoint();
    int                   durationMs = mRequest->durationms();

    UiDevice *device = UiDevice::getInstance(DeviceType::DEFAULT);

    device->drag(10, 200, 400, 400, durationMs);

    return grpc::Status::OK;
}