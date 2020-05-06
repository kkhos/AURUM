#include "TakeScreenshotCommand.h"
#include <loguru.hpp>
#include "UiObject.h"
#include "UiDevice.h"

TakeScreenshotCommand::TakeScreenshotCommand(
    const ::aurum::ReqTakeScreenshot *request,
    ::aurum::RspTakeScreenshot       *response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status TakeScreenshotCommand::execute()
{
    LOG_SCOPE_F(INFO, "TakeScreenshot --------------- ");

    UiDevice* mDevice = UiDevice::getInstance(DeviceType::DEFAULT);
    //TODO assign right path
    mDevice->takeScreenshot("/tmp/screenshot.png", 1.0, 1);

    return grpc::Status::OK;
}
