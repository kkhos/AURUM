/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *               http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#include "bootstrap.h"

class GetAttributeCommand : public Command {
protected:
    const ::aurum::ReqGetAttribute *mRequest;
    ::aurum::RspGetAttribute *mResponse;
    ObjectMapper *mObjMap;

public:
    GetAttributeCommand(const ::aurum::ReqGetAttribute *request,
                        ::aurum::RspGetAttribute *response);
    ::grpc::Status execute() override;
    static std::unique_ptr<GetAttributeCommand> createCommand(const ::aurum::ReqGetAttribute *request,
                                                              ::aurum::RspGetAttribute *response);
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