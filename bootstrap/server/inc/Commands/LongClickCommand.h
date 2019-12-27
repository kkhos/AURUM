#ifndef LONG_CLICK_COMMAND_H
#define LONG_CLICK_COMMAND_H

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include "aurum.grpc.pb.h"
#include "config.h"

class LongClickCommand : public Command {
private:
    const ::aurum::ReqClick* mRequest;
    ::aurum::RspClick*       mResponse;

public:
    LongClickCommand(const ::aurum::ReqClick* request,
                     ::aurum::RspClick*       response);
    ::grpc::Status execute() override;
private:
    const static unsigned int LOGNCLICK_INTERVAL = 50;
};

#endif