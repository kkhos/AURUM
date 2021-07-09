#pragma once

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include <aurum.grpc.pb.h>
#include "config.h"

class GetLocationCommand : public Command {
private:
    const ::aurum::ReqGetLocation *mRequest;
    ::aurum::RspGetLocation *mResponse;

public:
    GetLocationCommand(const ::aurum::ReqGetLocation *request,
                       ::aurum::RspGetLocation *response);
    ;
    ::grpc::Status execute() override;
};