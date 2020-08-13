#include "SyncCommand.h"
#include <loguru.hpp>

#include "UiDevice.h"

SyncCommand::SyncCommand(const ::aurum::ReqEmpty *request,
                         ::aurum::RspEmpty *      response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status SyncCommand::execute()
{
    std::shared_ptr<UiDevice> obj = UiDevice::getInstance();
    long long timeMs = obj->getSystemTime(TimeRequestType::WALLCLOCK);
    LOG_SCOPE_F(INFO, "Sync Command @ %f", timeMs/1000.0);
    // do post-command
    return grpc::Status::OK;
}