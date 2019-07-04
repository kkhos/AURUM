#ifndef IS_APP_INSTALLED_COMMAND_H
#define IS_APP_INSTALLED_COMMAND_H

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include "aurum.grpc.pb.h"
#include "config.h"

class GetAppInfoCommand : public Command {
private:
    const ::aurum::ReqGetAppInfo* mRequest;
    ::aurum::RspGetAppInfo*       mResponse;

public:
    GetAppInfoCommand(const ::aurum::ReqGetAppInfo* request,
                      ::aurum::RspGetAppInfo*       response);
    ;
    ::grpc::Status execute() override;
};

#endif