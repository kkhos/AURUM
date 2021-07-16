#pragma once

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include <aurum.grpc.pb.h>
#include "config.h"

class LaunchAppCommand : public Command {
private:
    const ::aurum::ReqLaunchApp *mRequest;
    ::aurum::RspLaunchApp *mResponse;

public:
    LaunchAppCommand(const ::aurum::ReqLaunchApp *request,
                     ::aurum::RspLaunchApp *response);
    ::grpc::Status execute() override;
};