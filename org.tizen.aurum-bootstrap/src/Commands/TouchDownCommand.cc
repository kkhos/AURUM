#include "TouchDownCommand.h"
#include <UiDevice.h>
#include <loguru.hpp>

TouchDownCommand::TouchDownCommand(const ::aurum::ReqTouchDown* request,
                                   ::aurum::RspTouchDown*       response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status TouchDownCommand::execute()
{
    LOG_SCOPE_F(INFO, "TouchDown --------------- ");
    const aurum::Point& point_ = mRequest->coordination();
    int seq = UiDevice::getInstance()
        ->touchDown(point_.x(), point_.y());
    mResponse->set_seqid(seq);
    return grpc::Status::OK;
}