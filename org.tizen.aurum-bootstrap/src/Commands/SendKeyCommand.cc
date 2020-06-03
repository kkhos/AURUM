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
    else if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_VOLUP)
        mDevice->pressVolUp();
    else if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_VOLDOWN)
        mDevice->pressVolDown();
    else if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_POWER)
        mDevice->pressPower();
    else if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_WHEELUP)
        mDevice->wheelUp(1,167);
    else if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_WHEELDOWN)
        mDevice->wheelDown(1,167);
    else if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_KEY) {
        ; // TODO
    }
    else if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_XF86) {
        ; // TODO
    }
    return grpc::Status::OK;
}