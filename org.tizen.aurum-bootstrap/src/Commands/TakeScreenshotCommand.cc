#include "TakeScreenshotCommand.h"
#include <loguru.hpp>
#include "UiObject.h"
#include "UiDevice.h"
#include <fstream>

TakeScreenshotCommand::TakeScreenshotCommand(
    const ::aurum::ReqTakeScreenshot*                  request,
    ::grpc::ServerWriter< ::aurum::RspTakeScreenshot>* writer)
    : mRequest{request}, mWriter{writer}
{
}

::grpc::Status TakeScreenshotCommand::execute()
{
    LOG_SCOPE_F(INFO, "TakeScreenshot --------------- ");

    std::string path = "/tmp/screenshot.png";
    std::shared_ptr<UiDevice> mDevice = UiDevice::getInstance();
    mDevice->takeScreenshot(path, 1.0, 1);

    std::ifstream ifs(path, std::ifstream::binary);
    ::aurum::RspTakeScreenshot rsp;
    int size = 1024 * 1024;
    char buf[size];

    while (!ifs.eof()) {
        ifs.read(buf, size);
        rsp.set_image(buf, ifs.gcount());
        mWriter->Write(rsp);
    }
    ifs.close();

    return grpc::Status::OK;
}
