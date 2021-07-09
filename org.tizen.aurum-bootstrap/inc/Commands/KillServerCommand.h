#pragma once

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include <aurum.grpc.pb.h>
#include "config.h"

class KillServerCommand : public Command {
private:
    const ::aurum::ReqEmpty *mRequest;
    ::aurum::RspEmpty *mResponse;

public:
    KillServerCommand(const ::aurum::ReqEmpty *request,
                      ::aurum::RspEmpty *response);
    ;
    ::grpc::Status execute() override;
};