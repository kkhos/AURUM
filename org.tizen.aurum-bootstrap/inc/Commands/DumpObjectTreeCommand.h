#pragma once
#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include "UiObject.h"

#include <aurum.grpc.pb.h>
#include "config.h"

class DumpObjectTreeCommand: public Command {
protected:
    const ::aurum::ReqDumpObjectTree* mRequest;
    ::aurum::RspDumpObjectTree*       mResponse;

protected:
    ObjectMapper* mObjMap;

public:
    DumpObjectTreeCommand(const ::aurum::ReqDumpObjectTree* request,
                       ::aurum::RspDumpObjectTree*       response);
    ::grpc::Status execute() override;

private:
    void traverse(::aurum::Element *root, std::shared_ptr<Node> node, int depth);
};
