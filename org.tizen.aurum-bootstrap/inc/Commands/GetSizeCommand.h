#pragma once

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include <aurum.grpc.pb.h>
#include "config.h"

class GetSizeCommand : public Command {
private:
    const ::aurum::ReqGetSize* mRequest;
    ::aurum::RspGetSize*       mResponse;

public:
    GetSizeCommand(const ::aurum::ReqGetSize* request,
                   ::aurum::RspGetSize*       response);
    ;
    ::grpc::Status execute() override;
};