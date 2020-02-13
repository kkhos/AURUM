#ifndef PRE_COMMAND_H
#define PRE_COMMAND_H

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "aurum.grpc.pb.h"
#include "config.h"

class PreCommand : public Command {
private:
    Command *mCommand;
    PreCommand();

public:
    PreCommand(Command *cmd);
    ::grpc::Status execute() override;
    ::grpc::Status executePost() override;
};

#endif