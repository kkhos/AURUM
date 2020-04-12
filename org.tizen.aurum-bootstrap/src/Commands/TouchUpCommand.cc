#include "TouchUpCommand.h"
#include <UiDevice.h>
#include <loguru.hpp>

TouchUpCommand::TouchUpCommand(const ::aurum::ReqTouchUp* request,
                               ::aurum::RspTouchUp*       response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status TouchUpCommand::execute()
{
    LOG_SCOPE_F(INFO, "TouchUp --------------- ");
    const aurum::Point& point = mRequest->coordination();
    UiDevice::getInstance(DeviceType::DEFAULT)->touchUp(point.x(), point.y());
    return grpc::Status::OK;
}