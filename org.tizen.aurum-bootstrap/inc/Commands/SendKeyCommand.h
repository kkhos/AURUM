#ifndef SEND_KEY_COMMAND_H
#define SEND_KEY_COMMAND_H

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include <aurum.grpc.pb.h>
#include "config.h"

class SendKeyCommand : public Command {
private:
    const ::aurum::ReqKey* mRequest;
    ::aurum::RspKey*       mResponse;

public:
    SendKeyCommand(const ::aurum::ReqKey* request, ::aurum::RspKey* response);
    ::grpc::Status execute() override;
};

#endif