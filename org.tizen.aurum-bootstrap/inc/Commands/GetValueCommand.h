#ifndef GET_VALUE_COMMAND_H
#define GET_VALUE_COMMAND_H

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include <aurum.grpc.pb.h>
#include "config.h"

class GetValueCommand : public Command {
private:
    const ::aurum::ReqGetValue* mRequest;
    ::aurum::RspGetValue*       mResponse;

public:
    GetValueCommand(const ::aurum::ReqGetValue* request,
                    ::aurum::RspGetValue*       response);
    ;
    ::grpc::Status execute() override;
};

#endif