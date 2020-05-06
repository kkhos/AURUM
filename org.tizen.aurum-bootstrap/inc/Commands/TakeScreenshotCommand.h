#ifndef TAKE_SCREENSHOT_COMMAND_H
#define TAKE_SCREENSHOT_COMMAND_H

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include <aurum.grpc.pb.h>
#include "config.h"

class TakeScreenshotCommand : public Command {
private:
    const ::aurum::ReqTakeScreenshot*                  mRequest;
    ::grpc::ServerWriter< ::aurum::RspTakeScreenshot>* mWriter;

public:
    TakeScreenshotCommand(const ::aurum::ReqTakeScreenshot*                  request,
                          ::grpc::ServerWriter< ::aurum::RspTakeScreenshot>* writer);
    ::grpc::Status execute() override;
};

#endif
