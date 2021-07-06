#include "bootstrap.h"
#include "SendKeyCommand.h"
#include "UiDevice.h"

SendKeyCommand::SendKeyCommand(const ::aurum::ReqKey* request,
                               ::aurum::RspKey*       response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status SendKeyCommand::execute()
{
    LOGI("SendKey --------------- ");
    std::shared_ptr<UiDevice> mDevice = UiDevice::getInstance();
    ::aurum::ReqKey_KeyType type = mRequest->type();
    ::aurum::ReqKey_KeyActionType action_type = mRequest->actiontype();
    KeyRequestType actionType = static_cast<KeyRequestType>(action_type);

    if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_BACK)
        mDevice->pressBack(actionType);
    else if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_HOME)
        mDevice->pressHome(actionType);
    else if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_MENU)
        mDevice->pressMenu(actionType);
    else if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_VOLUP)
        mDevice->pressVolUp(actionType);
    else if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_VOLDOWN)
        mDevice->pressVolDown(actionType);
    else if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_POWER)
        mDevice->pressPower(actionType);
    else if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_WHEELUP)
        mDevice->wheelUp(1, 167);
    else if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_WHEELDOWN)
        mDevice->wheelDown(1, 167);
    else if (type == ::aurum::ReqKey_KeyType::ReqKey_KeyType_XF86) {
        mDevice->pressKeyCode(mRequest->xf86keycode(), actionType);
    }
    return grpc::Status::OK;
}