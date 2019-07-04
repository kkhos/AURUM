#ifndef CLICK_COMMAND_H
#define CLICK_COMMAND_H

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include "aurum.grpc.pb.h"
#include "config.h"

class ClickCommand : public Command {
private:
    const ::aurum::ReqClick* mRequest;
    ::aurum::RspClick*       mResponse;

public:
    ClickCommand(const ::aurum::ReqClick* request, ::aurum::RspClick* response);
    ;
    ::grpc::Status execute() override;
};

#endif