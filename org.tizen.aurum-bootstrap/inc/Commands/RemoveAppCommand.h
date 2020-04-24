#ifndef REMOVE_APP_COMMAND_H
#define REMOVE_APP_COMMAND_H

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include <aurum.grpc.pb.h>
#include "config.h"

class RemoveAppCommand : public Command {
private:
    const ::aurum::ReqRemoveApp* mRequest;
    ::aurum::RspRemoveApp*       mResponse;

public:
    RemoveAppCommand(const ::aurum::ReqRemoveApp* request,
                     ::aurum::RspRemoveApp*       response);
    ;
    ::grpc::Status execute() override;
    ::grpc::Status executePost() override;
};

#endif