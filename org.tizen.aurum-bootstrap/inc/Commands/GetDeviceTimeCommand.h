#ifndef GET_DEVICE_TIME_COMMAND_H
#define GET_DEVICE_TIME_COMMAND_H

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include "aurum.grpc.pb.h"
#include "config.h"

class GetDeviceTimeCommand : public Command {
private:
    const ::aurum::ReqGetDeviceTime* mRequest;
    ::aurum::RspGetDeviceTime*       mResponse;

public:
    GetDeviceTimeCommand(const ::aurum::ReqGetDeviceTime* request,
                         ::aurum::RspGetDeviceTime*       response);
    ;
    ::grpc::Status execute() override;
};

#endif