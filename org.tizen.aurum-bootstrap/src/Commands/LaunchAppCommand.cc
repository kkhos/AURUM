#include "bootstrap.h"
#include "LaunchAppCommand.h"
#include "LaunchAppRunnable.h"
#include "UiDevice.h"
#include <chrono>
#include <thread>

#define WAIT_APP_LAUNCH 10000

LaunchAppCommand::LaunchAppCommand(const ::aurum::ReqLaunchApp *request,
                                   ::aurum::RspLaunchApp *response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status LaunchAppCommand::execute()
{
    LOGI("LaunchApp --------------- ");
    std::unique_ptr<LaunchAppRunnable> cmd = std::make_unique<LaunchAppRunnable>(mRequest->packagename(), mRequest->data());
    std::shared_ptr<UiDevice> obj = UiDevice::getInstance();
    obj->executeAndWaitForEvents(cmd.get(), A11yEvent::EVENT_WINDOW_ACTIVATE, WAIT_APP_LAUNCH);

    return grpc::Status::OK;
}