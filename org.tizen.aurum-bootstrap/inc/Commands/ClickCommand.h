#pragma once

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include <aurum.grpc.pb.h>
#include "config.h"
class ClickCommand : public Command {
protected:
    const ::aurum::ReqClick* mRequest;
    ::aurum::RspClick*       mResponse;

public:
    ClickCommand(const ::aurum::ReqClick* request, ::aurum::RspClick* response);
    ::grpc::Status execute() override;
    static std::unique_ptr<ClickCommand> createCommand(const ::aurum::ReqClick* request, ::aurum::RspClick* response);
};

class ClickElementCommand : public ClickCommand {
    using ClickCommand::ClickCommand;
public:
    ::grpc::Status execute() override;
};

class ClickCoordCommand : public ClickCommand {
    using ClickCommand::ClickCommand;
public:
    ::grpc::Status execute() override;
};

class ClickAtspiCommand : public ClickCommand {
    using ClickCommand::ClickCommand;
public:
    ::grpc::Status execute() override;
};