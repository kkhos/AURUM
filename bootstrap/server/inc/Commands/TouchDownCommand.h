#ifndef TOUCH_DOWN_COMMAND_H
#define TOUCH_DOWN_COMMAND_H

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include "aurum.grpc.pb.h"
#include "config.h"

class TouchDownCommand : public Command {
private:
    const ::aurum::ReqTouchDown* mRequest;
    ::aurum::RspTouchDown*       mResponse;

public:
    TouchDownCommand(const ::aurum::ReqTouchDown* request,
                     ::aurum::RspTouchDown*       response);
    ::grpc::Status execute() override;
};

#endif