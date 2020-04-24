#ifndef SET_VALUE_COMMAND_H
#define SET_VALUE_COMMAND_H

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include <aurum.grpc.pb.h>
#include "config.h"

class SetValueCommand : public Command {
private:
    const ::aurum::ReqSetValue* mRequest;
    ::aurum::RspSetValue*       mResponse;

public:
    SetValueCommand(const ::aurum::ReqSetValue* request,
                    ::aurum::RspSetValue*       response);
    ::grpc::Status execute() override;
};

#endif