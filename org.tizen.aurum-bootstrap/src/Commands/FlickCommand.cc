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

    const ::aurum::Point &startPoint = mRequest->startpoint();
    const ::aurum::Point &endPoint = mRequest->endpoint();
    int                   durationMs = mRequest->durationms();

    UiDevice *device = UiDevice::getInstance(DeviceType::DEFAULT);
    device->drag(startPoint.x(), startPoint.y(), endPoint.x(), endPoint.y(), durationMs);
    mResponse->set_status(::aurum::RspStatus::OK);

    return grpc::Status::OK;
}