#include <Aurum.h>
#include "FlickCommand.h"

FlickCommand::FlickCommand(const ::aurum::ReqFlick *request,
                           ::aurum::RspFlick *      response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status FlickCommand::execute()
{
    LOGI("Flick --------------- ");

    const ::aurum::Point &startPoint = mRequest->startpoint();
    const ::aurum::Point &endPoint = mRequest->endpoint();
    int                   durationMs = mRequest->durationms();

    std::shared_ptr<UiDevice> device = UiDevice::getInstance();
    device->drag(startPoint.x(), startPoint.y(), endPoint.x(), endPoint.y(), durationMs/MINIMUM_TOUCHEVENT_INTV_MS, durationMs);
    mResponse->set_status(::aurum::RspStatus::OK);

    return grpc::Status::OK;
}