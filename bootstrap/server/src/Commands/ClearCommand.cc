#include "ClearCommand.h"
#include <UiObject.h>
#include <loguru.hpp>

#include <string>

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
        std::string empty{};
        obj->setText(empty);
        mResponse->set_status(::aurum::RspStatus::OK);
    }

    return grpc::Status::OK;
}