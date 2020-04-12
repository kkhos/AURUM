#ifndef FLICK_COMMAND_H
#define FLICK_COMMAND_H

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include "aurum.grpc.pb.h"
#include "config.h"

class FlickCommand : public Command {
private:
    const ::aurum::ReqFlick* mRequest;
    ::aurum::RspFlick*       mResponse;

public:
    FlickCommand(const ::aurum::ReqFlick* request, ::aurum::RspFlick* response);
    ;
    ::grpc::Status execute() override;
};

#endif