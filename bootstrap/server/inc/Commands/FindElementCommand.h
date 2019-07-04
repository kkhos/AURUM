#ifndef FIND_ELEMENT_COMMAND_H
#define FIND_ELEMENT_COMMAND_H

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include "aurum.grpc.pb.h"
#include "config.h"

class FindElementCommand : public Command {
private:
    const ::aurum::ReqFindElement* mRequest;
    ::aurum::RspFindElement*       mResponse;

private:
    ObjectMapper* mObjMap;

public:
    FindElementCommand(const ::aurum::ReqFindElement* request,
                       ::aurum::RspFindElement*       response);
    ::grpc::Status execute() override;
};

#endif