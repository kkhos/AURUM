#include "SyncCommand.h"
#include <loguru.hpp>

#include <Accessible.h>
#include <AccessibleNode.h>

SyncCommand::SyncCommand(const ::aurum::ReqEmpty *request,
                         ::aurum::RspEmpty *      response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status SyncCommand::execute()
{
    LOG_SCOPE_F(INFO, "Sync Command ");
    const Accessible *accObj = Accessible::getInstance();

    AccessibleNode *root = accObj->getRootNode();
    AccessibleNode *top = accObj->getTopNode();
    LOG_F(INFO, "%p(%p) %p(%p)", root, root->getAccessible(), top,
          top->getAccessible());

    root->print(0, 2);
    LOG_F(INFO, "---------");
    top->print(0, 2);

    return grpc::Status::OK;
}