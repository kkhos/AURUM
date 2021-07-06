#include <Aurum.h>
#include "SetValueCommand.h"

SetValueCommand::SetValueCommand(const ::aurum::ReqSetValue* request,
                                 ::aurum::RspSetValue*       response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status SetValueCommand::execute()
{
    LOGI("SetValue (text:%s) --------------- ", mRequest->stringvalue().c_str());
    ObjectMapper* mObjMap = ObjectMapper::getInstance();
    std::shared_ptr<UiObject> obj = mObjMap->getElement(mRequest->elementid());
    if (obj) obj->setText(const_cast<std::string&>(mRequest->stringvalue()));
    return grpc::Status::OK;
}