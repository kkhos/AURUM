#ifndef TOUCH_UP_COMMAND_H
#define TOUCH_UP_COMMAND_H

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include "aurum.grpc.pb.h"
#include "config.h"

class TouchUpCommand : public Command {
private:
    const ::aurum::ReqTouchUp* mRequest;
    ::aurum::RspTouchUp*       mResponse;

public:
    TouchUpCommand(const ::aurum::ReqTouchUp* request,
                   ::aurum::RspTouchUp*       response);
    ::grpc::Status execute() override;
};

#endif