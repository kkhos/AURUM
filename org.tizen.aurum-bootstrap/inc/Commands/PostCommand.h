#pragma once

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include <aurum.grpc.pb.h>
#include "config.h"

class PostCommand : public Command {
private:
    Command *mCommand;
    PostCommand();

public:
    PostCommand(Command *cmd);
    ::grpc::Status execute() override;
    ::grpc::Status executePre() override;
};