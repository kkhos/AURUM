#include "SendKeyCommand.h"
#include <loguru.hpp>

#include <UiDevice.h>

SendKeyCommand::SendKeyCommand(const ::aurum::ReqKey* request,
                               ::aurum::RspKey*       response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status SendKeyCommand::execute()
{
    LOG_SCOPE_F(INFO, "SendKey --------------- ");
    UiDevice* mDevice = UiDevice::getInstance(DeviceType::DEFAULT);
    ::aurum::ReqKey_KeyType type = mRequest->type();

    if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_BACK)
        mDevice->pressBack();
    else if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_HOME)
        mDevice->pressHome();
    else if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_MENU)
        mDevice->pressMenu();
    else {
        // TODO : handle keycode
    }
    return grpc::Status::OK;
}