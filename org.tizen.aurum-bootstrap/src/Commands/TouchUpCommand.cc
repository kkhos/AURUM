#include "bootstrap.h"
#include "TouchUpCommand.h"
#include "UiDevice.h"

TouchUpCommand::TouchUpCommand(const ::aurum::ReqTouchUp *request,
                               ::aurum::RspTouchUp *response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status TouchUpCommand::execute()
{
    LOGI("TouchUp --------------- ");
    const aurum::Point& point = mRequest->coordination();
    int seq = mRequest->seqid();
    UiDevice::getInstance()->touchUp(point.x(), point.y(), seq);
    return grpc::Status::OK;
}