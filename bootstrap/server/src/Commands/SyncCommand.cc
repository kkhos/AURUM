#include "SyncCommand.h"
#include <loguru.hpp>

#include <AccessibleWatcher.h>
#include <AccessibleNode.h>

SyncCommand::SyncCommand(const ::aurum::ReqEmpty *request,
                         ::aurum::RspEmpty *      response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status SyncCommand::execute()
{
    LOG_SCOPE_F(INFO, "Sync Command ");
    const AccessibleWatcher *accObj = AccessibleWatcher::getInstance();

    auto root = accObj->getRootNode();
    LOG_F(INFO, "root node tree / depth : 2");
    root->print(0, 2);

    return grpc::Status::OK;
}