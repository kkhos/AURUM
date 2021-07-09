#include "bootstrap.h"
#include "TouchMoveCommand.h"
#include "UiDevice.h"

TouchMoveCommand::TouchMoveCommand(const ::aurum::ReqTouchMove *request,
                                   ::aurum::RspTouchMove *response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status TouchMoveCommand::execute()
{
    LOGI("TouchMove --------------- ");
    const aurum::Point& point = mRequest->coordination();
    int seq = mRequest->seqid();
    UiDevice::getInstance()->touchMove(point.x(), point.y(), seq);
    return grpc::Status::OK;
}