#ifndef CLOSE_APP_COMMAND_H
#define CLOSE_APP_COMMAND_H

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include "aurum.grpc.pb.h"
#include "config.h"

class CloseAppCommand : public Command {
private:
    const ::aurum::ReqCloseApp* mRequest;
    ::aurum::RspCloseApp*       mResponse;

public:
    CloseAppCommand(const ::aurum::ReqCloseApp* request,
                    ::aurum::RspCloseApp*       response);
    ::grpc::Status execute() override;
    ::grpc::Status executePost() override;
};

#endif