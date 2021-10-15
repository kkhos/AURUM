/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd All Rights Reserved
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

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include "UiObject.h"

#include <aurum.grpc.pb.h>
#include "config.h"

class DumpObjectTreeCommand: public Command {
protected:
    const ::aurum::ReqDumpObjectTree *mRequest;
    ::aurum::RspDumpObjectTree *mResponse;

protected:
    ObjectMapper *mObjMap;

public:
    DumpObjectTreeCommand(const ::aurum::ReqDumpObjectTree *request,
                       ::aurum::RspDumpObjectTree *response);
    ::grpc::Status execute() override;

private:
    void traverse(::aurum::Element *root, std::shared_ptr<Node> node, int depth);
};
