#ifndef GET_ATTRIBUTE_COMMAND_H
#define GET_ATTRIBUTE_COMMAND_H

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include <aurum.grpc.pb.h>
#include "config.h"

class GetAttributeCommand : public Command {
protected:
    const ::aurum::ReqGetAttribute* mRequest;
    ::aurum::RspGetAttribute*       mResponse;
    ObjectMapper* mObjMap;

public:
    GetAttributeCommand(const ::aurum::ReqGetAttribute* request,
                        ::aurum::RspGetAttribute*       response);
    ::grpc::Status execute() override;
    static std::unique_ptr<GetAttributeCommand> createCommand(const ::aurum::ReqGetAttribute* request,
                                                              ::aurum::RspGetAttribute*       response);
};

class GetVisibleAttributeCommand : public GetAttributeCommand {
    using GetAttributeCommand::GetAttributeCommand;
public:
    ::grpc::Status execute() override;
};

class GetFocusedAttributeCommand : public GetAttributeCommand {
    using GetAttributeCommand::GetAttributeCommand;
public:
    ::grpc::Status execute() override;
};

class GetFocusableAttributeCommand : public GetAttributeCommand {
    using GetAttributeCommand::GetAttributeCommand;
public:
    ::grpc::Status execute() override;
};

class GetCheckableAttributeCommand : public GetAttributeCommand {
    using GetAttributeCommand::GetAttributeCommand;
public:
    ::grpc::Status execute() override;
};

class GetCheckedAttributeCommand : public GetAttributeCommand {
    using GetAttributeCommand::GetAttributeCommand;
public:
    ::grpc::Status execute() override;
};

class GetEnabledAttributeCommand : public GetAttributeCommand {
    using GetAttributeCommand::GetAttributeCommand;
public:
    ::grpc::Status execute() override;
};

class GetClickableAttributeCommand : public GetAttributeCommand {
    using GetAttributeCommand::GetAttributeCommand;
public:
    ::grpc::Status execute() override;
};

class GetScrollableAttributeCommand : public GetAttributeCommand {
    using GetAttributeCommand::GetAttributeCommand;
public:
    ::grpc::Status execute() override;
};

class GetSelectableAttributeCommand : public GetAttributeCommand {
    using GetAttributeCommand::GetAttributeCommand;
public:
    ::grpc::Status execute() override;
};

class GetSelectedAttributeCommand : public GetAttributeCommand {
    using GetAttributeCommand::GetAttributeCommand;
public:
    ::grpc::Status execute() override;
};

class GetShowingAttributeCommand : public GetAttributeCommand {
    using GetAttributeCommand::GetAttributeCommand;
public:
    ::grpc::Status execute() override;
};

class GetActiveAttributeCommand : public GetAttributeCommand {
    using GetAttributeCommand::GetAttributeCommand;
public:
    ::grpc::Status execute() override;
};
#endif