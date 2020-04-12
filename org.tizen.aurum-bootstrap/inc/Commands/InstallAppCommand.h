#ifndef INSTALL_APP_COMMAND_H
#define INSTALL_APP_COMMAND_H

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include "aurum.grpc.pb.h"
#include "config.h"

class InstallAppCommand : public Command {
private:
    ::grpc::ServerReader< ::aurum::ReqInstallApp>* mRequest;
    ::aurum::RspInstallApp*                        mResponse;

public:
    InstallAppCommand(::grpc::ServerReader< ::aurum::ReqInstallApp>* request,
                      ::aurum::RspInstallApp*                        response);
    ::grpc::Status execute() override;
};

#endif