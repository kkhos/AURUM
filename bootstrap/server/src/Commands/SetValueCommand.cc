#include "SetValueCommand.h"
#include <loguru.hpp>

SetValueCommand::SetValueCommand(const ::aurum::ReqSetValue* request,
                                 ::aurum::RspSetValue*       response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status SetValueCommand::execute()
{
    LOG_SCOPE_F(INFO, "SetValue --------------- ");
    ObjectMapper* mObjMap = ObjectMapper::getInstance();
    UiObject*     obj = mObjMap->getElement(mRequest->elementid());
    obj->setText(const_cast<std::string&>(mRequest->stringvalue()));
    LOG_F(INFO, "%p %s", obj, mRequest->stringvalue().c_str());

    return grpc::Status::OK;
}