/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *               http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#include "bootstrap.h"
#include "GetAttributeCommand.h"
#include "UiObject.h"
#include "UiDevice.h"

GetAttributeCommand::GetAttributeCommand(
    const ::aurum::ReqGetAttribute *request, ::aurum::RspGetAttribute *response)
    : mRequest{request}, mResponse{response},  mObjMap{ObjectMapper::getInstance()}
{
}

::grpc::Status GetAttributeCommand::execute()
{
    return grpc::Status::CANCELLED;
}

std::unique_ptr<GetAttributeCommand> GetAttributeCommand::createCommand(const ::aurum::ReqGetAttribute *request,
                                                                        ::aurum::RspGetAttribute*response)
{
    LOGI("GetAttribute --------------- ");
    ::aurum::ReqGetAttribute_RequestType type = request->attribute();
    LOGI("type : %d", type);

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
    else if (type == ::aurum::ReqGetAttribute_RequestType::ReqGetAttribute_RequestType_SHOWING)
        return std::make_unique<GetShowingAttributeCommand>(request, response);
    else if (type == ::aurum::ReqGetAttribute_RequestType::ReqGetAttribute_RequestType_ACTIVE)
        return std::make_unique<GetActiveAttributeCommand>(request, response);
    else
        return std::make_unique<GetAttributeCommand>(request, response);
}

::grpc::Status GetVisibleAttributeCommand::execute()
{
    std::shared_ptr<UiObject> obj = mObjMap->getElement(mRequest->elementid());
    if (!obj) {
        mResponse->set_boolvalue(false);
        mResponse->set_status(aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }
    obj->updateStates();
    mResponse->set_boolvalue(obj->isVisible());
    mResponse->set_status(aurum::RspStatus::OK);
    return grpc::Status::OK;
}

::grpc::Status GetFocusedAttributeCommand::execute()
{
    std::shared_ptr<UiObject> obj = mObjMap->getElement(mRequest->elementid());
    if (!obj) {
        mResponse->set_boolvalue(false);
        mResponse->set_status(aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }
    obj->updateStates();
    mResponse->set_boolvalue(obj->isFocused());
    mResponse->set_status(aurum::RspStatus::OK);
    return grpc::Status::OK;
}

::grpc::Status GetFocusableAttributeCommand::execute()
{
    std::shared_ptr<UiObject> obj = mObjMap->getElement(mRequest->elementid());
    if (!obj) {
        mResponse->set_boolvalue(false);
        mResponse->set_status(aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }
    obj->updateStates();
    mResponse->set_boolvalue(obj->isFocusable());
    mResponse->set_status(aurum::RspStatus::OK);
    return grpc::Status::OK;
}

::grpc::Status GetCheckableAttributeCommand::execute()
{
    std::shared_ptr<UiObject> obj = mObjMap->getElement(mRequest->elementid());
    if (!obj) {
        mResponse->set_boolvalue(false);
        mResponse->set_status(aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }
    obj->updateStates();
    mResponse->set_boolvalue(obj->isCheckable());
    mResponse->set_status(aurum::RspStatus::OK);
    return grpc::Status::OK;
}

::grpc::Status GetCheckedAttributeCommand::execute()
{
    std::shared_ptr<UiObject> obj = mObjMap->getElement(mRequest->elementid());
    if (!obj) {
        mResponse->set_boolvalue(false);
        mResponse->set_status(aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }
    obj->updateStates();
    mResponse->set_boolvalue(obj->isChecked());
    mResponse->set_status(aurum::RspStatus::OK);
    return grpc::Status::OK;
}

::grpc::Status GetEnabledAttributeCommand::execute()
{
    std::shared_ptr<UiObject> obj = mObjMap->getElement(mRequest->elementid());
    if (!obj) {
        mResponse->set_boolvalue(false);
        mResponse->set_status(aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }
    obj->updateStates();
    mResponse->set_boolvalue(obj->isEnabled());
    mResponse->set_status(aurum::RspStatus::OK);
    return grpc::Status::OK;
}

::grpc::Status GetClickableAttributeCommand::execute()
{
    std::shared_ptr<UiObject> obj = mObjMap->getElement(mRequest->elementid());
    if (!obj) {
        mResponse->set_boolvalue(false);
        mResponse->set_status(aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }
    obj->updateStates();
    mResponse->set_boolvalue(obj->isClickable());
    mResponse->set_status(aurum::RspStatus::OK);
    return grpc::Status::OK;
}

::grpc::Status GetScrollableAttributeCommand::execute()
{
    std::shared_ptr<UiObject> obj = mObjMap->getElement(mRequest->elementid());
    if (!obj) {
        mResponse->set_boolvalue(false);
        mResponse->set_status(aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }
    obj->updateStates();
    mResponse->set_boolvalue(obj->isScrollable());
    mResponse->set_status(aurum::RspStatus::OK);
    return grpc::Status::OK;
}

::grpc::Status GetSelectableAttributeCommand::execute()
{
    std::shared_ptr<UiObject> obj = mObjMap->getElement(mRequest->elementid());
    if (!obj) {
        mResponse->set_boolvalue(false);
        mResponse->set_status(aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }
    obj->updateStates();
    mResponse->set_boolvalue(obj->isSelectable());
    mResponse->set_status(aurum::RspStatus::OK);
    return grpc::Status::OK;
}

::grpc::Status GetSelectedAttributeCommand::execute()
{
    std::shared_ptr<UiObject> obj = mObjMap->getElement(mRequest->elementid());
    if (!obj) {
        mResponse->set_boolvalue(false);
        mResponse->set_status(aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }
    obj->updateStates();
    mResponse->set_boolvalue(obj->isSelected());
    mResponse->set_status(aurum::RspStatus::OK);
    return grpc::Status::OK;
}

::grpc::Status GetShowingAttributeCommand::execute()
{
    std::shared_ptr<UiObject> obj = mObjMap->getElement(mRequest->elementid());
    if (!obj) {
        mResponse->set_boolvalue(false);
        mResponse->set_status(aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }
    obj->updateStates();
    mResponse->set_boolvalue(obj->isShowing());
    mResponse->set_status(aurum::RspStatus::OK);
    return grpc::Status::OK;
}

::grpc::Status GetActiveAttributeCommand::execute()
{
    std::shared_ptr<UiObject> obj = mObjMap->getElement(mRequest->elementid());
    if (!obj) {
        mResponse->set_boolvalue(false);
        mResponse->set_status(aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }
    obj->updateStates();
    mResponse->set_boolvalue(obj->isActive());
    mResponse->set_status(aurum::RspStatus::OK);
    return grpc::Status::OK;
}
