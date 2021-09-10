#include "bootstrap.h"
#include "TakeScreenshotCommand.h"
#include <fstream>
#include "UiObject.h"
#include "UiDevice.h"

TakeScreenshotCommand::TakeScreenshotCommand(
    const ::aurum::ReqTakeScreenshot *request,
    ::grpc::ServerWriter< ::aurum::RspTakeScreenshot> *writer)
    : mRequest{request}, mWriter{writer}
{
}

::grpc::Status TakeScreenshotCommand::execute()
{
    LOGI("TakeScreenshot --------------- ");

    struct tm timeinfo;
    time_t now = time(0);
    if (!localtime_r(&now, &timeinfo))
        LOGE("fail to get localtime");

    char name[128];
    std::snprintf(name, 128, "/tmp/screenshot-%d-%d-%d-%d:%d:%d.png",
                              (timeinfo.tm_year + 1900), (timeinfo.tm_mon + 1), timeinfo.tm_mday,
                              timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    std::string path(name);
    std::shared_ptr<UiDevice> mDevice = UiDevice::getInstance();
    mDevice->takeScreenshot(path, 1.0, 1);

    std::ifstream ifs(path, std::ifstream::binary);
    ::aurum::RspTakeScreenshot rsp;
    int size = mDevice->getScreenSize().width() * mDevice->getScreenSize().height();
    char buf[size];

    while (!ifs.eof()) {
        ifs.read(buf, size);
        rsp.set_image(buf, ifs.gcount());
        mWriter->Write(rsp);
    }
    ifs.close();

    return grpc::Status::OK;
}
