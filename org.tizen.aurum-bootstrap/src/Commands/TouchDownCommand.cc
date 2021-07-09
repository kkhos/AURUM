#include "bootstrap.h"
#include "TouchDownCommand.h"
#include "UiDevice.h"

TouchDownCommand::TouchDownCommand(const ::aurum::ReqTouchDown *request,
                                   ::aurum::RspTouchDown *response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status TouchDownCommand::execute()
{
    LOGI("TouchDown --------------- ");
    const aurum::Point& point_ = mRequest->coordination();
    int seq = UiDevice::getInstance()
        ->touchDown(point_.x(), point_.y());
    mResponse->set_seqid(seq);
    return grpc::Status::OK;
}