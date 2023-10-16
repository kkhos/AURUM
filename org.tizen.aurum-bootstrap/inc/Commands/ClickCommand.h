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

class ClickCommand : public Command {
protected:
    const ::aurum::ReqClick *mRequest;
    ::aurum::RspClick *mResponse;

public:
    ClickCommand(const ::aurum::ReqClick *request, ::aurum::RspClick *response);
    ::grpc::Status execute() override;
    static std::unique_ptr<ClickCommand> createCommand(const ::aurum::ReqClick *request, ::aurum::RspClick *response);
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