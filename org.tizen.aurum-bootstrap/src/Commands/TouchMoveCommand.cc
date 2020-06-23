#include "TouchMoveCommand.h"
#include <UiDevice.h>
#include <loguru.hpp>

TouchMoveCommand::TouchMoveCommand(const ::aurum::ReqTouchMove* request,
                                   ::aurum::RspTouchMove*       response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status TouchMoveCommand::execute()
{
    LOG_SCOPE_F(INFO, "TouchMove --------------- ");
    const aurum::Point& point = mRequest->coordination();
    int seq = mRequest->seqid();
    UiDevice::getInstance(DeviceType::DEFAULT)->touchMove(point.x(), point.y(), seq);
    return grpc::Status::OK;
}