#ifndef SYNC_COMMAND_H
#define SYNC_COMMAND_H

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include <aurum.grpc.pb.h>
#include "config.h"

class SyncCommand : public Command {
private:
    const ::aurum::ReqEmpty* mRequest;
    ::aurum::RspEmpty*       mResponse;

public:
    SyncCommand(const ::aurum::ReqEmpty* request, ::aurum::RspEmpty* response);
    ::grpc::Status execute() override;
};

#endif