#ifndef TOUCH_MOVE_COMMAND_H
#define TOUCH_MOVE_COMMAND_H

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include <aurum.grpc.pb.h>
#include "config.h"

class TouchMoveCommand : public Command {
private:
    const ::aurum::ReqTouchMove* mRequest;
    ::aurum::RspTouchMove*       mResponse;

public:
    TouchMoveCommand(const ::aurum::ReqTouchMove* request,
                     ::aurum::RspTouchMove*       response);
    ::grpc::Status execute() override;
};

#endif