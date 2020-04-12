#ifndef CLEAR_COMMAND_H
#define CLEAR_COMMAND_H

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include "aurum.grpc.pb.h"
#include "config.h"

class ClearCommand : public Command {
private:
    const ::aurum::ReqClear* mRequest;
    ::aurum::RspClear*       mResponse;

public:
    ClearCommand(const ::aurum::ReqClear* request, ::aurum::RspClear* response);
    ::grpc::Status execute() override;

protected:
    bool hasHintText(UiObject *obj);
};

#endif