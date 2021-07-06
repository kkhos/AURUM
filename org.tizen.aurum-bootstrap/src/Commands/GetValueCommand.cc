#include <Aurum.h>
#include "GetValueCommand.h"

GetValueCommand::GetValueCommand(const ::aurum::ReqGetValue* request,
                                 ::aurum::RspGetValue*       response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status GetValueCommand::execute()
{
    LOGI("GetValue --------------- ");
    ObjectMapper* mObjMap = ObjectMapper::getInstance();
    std::shared_ptr<UiObject> obj = mObjMap->getElement(mRequest->elementid());

    if (obj) {
        std::string text = obj->getText();
        mResponse->set_stringvalue(text.c_str());
        mResponse->set_status(::aurum::RspStatus::OK);
    }

    return grpc::Status::OK;
}