#include "GetAttributeCommand.h"
#include <loguru.hpp>

#include <UiDevice.h>
#include <UiObject.h>

GetAttributeCommand::GetAttributeCommand(
    const ::aurum::ReqGetAttribute* request, ::aurum::RspGetAttribute* response)
    : mRequest{request}, mResponse{response},  mObjMap{ObjectMapper::getInstance()}
{
}

::grpc::Status GetAttributeCommand::execute()
{
    return grpc::Status::CANCELLED;
}

std::unique_ptr<GetAttributeCommand> GetAttributeCommand::createCommand(const ::aurum::ReqGetAttribute* request,
                                                                        ::aurum::RspGetAttribute*       response)
{
    ::aurum::ReqGetAttribute_RequestType type = request->attribute();

    LOG_SCOPE_F(INFO, "type : %d", type);

    if (type == ::aurum::ReqGetAttribute_RequestType::ReqGetAttribute_RequestType_VISIBLE)
        return std::make_unique<GetVisibleAttributeCommand>(request, response);
    else if (type == ::aurum::ReqGetAttribute_RequestType::ReqGetAttribute_RequestType_FOCUSABLE)
        return std::make_unique<GetFocusableAttributeCommand>(request, response);
    else if (type == ::aurum::ReqGetAttribute_RequestType::ReqGetAttribute_RequestType_FOCUSED)
        return std::make_unique<GetFocusedAttributeCommand>(request, response);
    else if (type == ::aurum::ReqGetAttribute_RequestType::ReqGetAttribute_RequestType_ENABLED)
        return std::make_unique<GetEnabledAttributeCommand>(request, response);
    else if (type == ::aurum::ReqGetAttribute_RequestType::ReqGetAttribute_RequestType_CLICKABLE)
        return std::make_unique<GetClickableAttributeCommand>(request, response);
    else if (type == ::aurum::ReqGetAttribute_RequestType::ReqGetAttribute_RequestType_SCROLLABLE)
        return std::make_unique<GetScrollableAttributeCommand>(request, response);
    else if (type == ::aurum::ReqGetAttribute_RequestType::ReqGetAttribute_RequestType_CHECKABLE)
        return std::make_unique<GetCheckableAttributeCommand>(request, response);
    else if (type == ::aurum::ReqGetAttribute_RequestType::ReqGetAttribute_RequestType_CHECKED)
        return std::make_unique<GetCheckedAttributeCommand>(request, response);
    else if (type == ::aurum::ReqGetAttribute_RequestType::ReqGetAttribute_RequestType_SELECTED)
        return std::make_unique<GetSelectedAttributeCommand>(request, response);
    else if (type == ::aurum::ReqGetAttribute_RequestType::ReqGetAttribute_RequestType_SELECTABLE)
        return std::make_unique<GetSelectableAttributeCommand>(request, response);
    else
        return std::make_unique<GetAttributeCommand>(request, response);
}

::grpc::Status GetVisibleAttributeCommand::execute()
{
    UiObject* obj = mObjMap->getElement(mRequest->elementid());
    if (!obj) {
        mResponse->set_boolvalue(false);
        mResponse->set_status(aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }
    mResponse->set_boolvalue(obj->isVisible());
    mResponse->set_status(aurum::RspStatus::OK);
    return grpc::Status::OK;
}

::grpc::Status GetFocusedAttributeCommand::execute()
{
    UiObject* obj = mObjMap->getElement(mRequest->elementid());
    if (!obj) {
        mResponse->set_boolvalue(false);
        mResponse->set_status(aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }
    mResponse->set_boolvalue(obj->isFocused());
    mResponse->set_status(aurum::RspStatus::OK);
    return grpc::Status::OK;
}

::grpc::Status GetFocusableAttributeCommand::execute()
{
    UiObject* obj = mObjMap->getElement(mRequest->elementid());
    if (!obj) {
        mResponse->set_boolvalue(false);
        mResponse->set_status(aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }
    mResponse->set_boolvalue(obj->isFocusable());
    mResponse->set_status(aurum::RspStatus::OK);
    return grpc::Status::OK;
}

::grpc::Status GetCheckableAttributeCommand::execute()
{
    UiObject* obj = mObjMap->getElement(mRequest->elementid());
    if (!obj) {
        mResponse->set_boolvalue(false);
        mResponse->set_status(aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }
    mResponse->set_boolvalue(obj->isCheckable());
    mResponse->set_status(aurum::RspStatus::OK);
    return grpc::Status::OK;
}

::grpc::Status GetCheckedAttributeCommand::execute()
{
    UiObject* obj = mObjMap->getElement(mRequest->elementid());
    if (!obj) {
        mResponse->set_boolvalue(false);
        mResponse->set_status(aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }
    mResponse->set_boolvalue(obj->isChecked());
    mResponse->set_status(aurum::RspStatus::OK);
    return grpc::Status::OK;
}

::grpc::Status GetEnabledAttributeCommand::execute()
{
    UiObject* obj = mObjMap->getElement(mRequest->elementid());
    if (!obj) {
        mResponse->set_boolvalue(false);
        mResponse->set_status(aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }
    mResponse->set_boolvalue(obj->isEnabled());
    mResponse->set_status(aurum::RspStatus::OK);
    return grpc::Status::OK;
}

::grpc::Status GetClickableAttributeCommand::execute()
{
    UiObject* obj = mObjMap->getElement(mRequest->elementid());
    if (!obj) {
        mResponse->set_boolvalue(false);
        mResponse->set_status(aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }
    mResponse->set_boolvalue(obj->isClickable());
    mResponse->set_status(aurum::RspStatus::OK);
    return grpc::Status::OK;
}

::grpc::Status GetScrollableAttributeCommand::execute()
{
    UiObject* obj = mObjMap->getElement(mRequest->elementid());
    if (!obj) {
        mResponse->set_boolvalue(false);
        mResponse->set_status(aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }
    mResponse->set_boolvalue(obj->isScrollable());
    mResponse->set_status(aurum::RspStatus::OK);
    return grpc::Status::OK;
}

::grpc::Status GetSelectableAttributeCommand::execute()
{
    UiObject* obj = mObjMap->getElement(mRequest->elementid());
    if (!obj) {
        mResponse->set_boolvalue(false);
        mResponse->set_status(aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }
    mResponse->set_boolvalue(obj->isSelectable());
    mResponse->set_status(aurum::RspStatus::OK);
    return grpc::Status::OK;
}

::grpc::Status GetSelectedAttributeCommand::execute()
{
    UiObject* obj = mObjMap->getElement(mRequest->elementid());
    if (!obj) {
        mResponse->set_boolvalue(false);
        mResponse->set_status(aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }
    mResponse->set_boolvalue(obj->isSelected());
    mResponse->set_status(aurum::RspStatus::OK);
    return grpc::Status::OK;
}
