#include "ClearCommand.h"
#include <UiObject.h>
#include <loguru.hpp>

ClearCommand::ClearCommand(const ::aurum::ReqClear* request,
                           ::aurum::RspClear*       response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status ClearCommand::execute()
{
    LOG_SCOPE_F(INFO, "Clear --------------- ");
    ObjectMapper* mObjMap = ObjectMapper::getInstance();
    UiObject*     obj = mObjMap->getElement(mRequest->elementid());
    if (obj) {
        ;
    }
    return grpc::Status::OK;
}