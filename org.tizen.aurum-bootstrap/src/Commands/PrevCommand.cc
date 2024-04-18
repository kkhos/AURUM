/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd All Rights Reserved
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

#include "PrevCommand.h"
#include "UiObject.h"

PrevCommand::PrevCommand(const ::aurum::ReqPrev *request,
                               ::aurum::RspPrev *response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status PrevCommand::execute()
{
    LOGI("Prev --------------- ");

    ObjectMapper *mObjMap = ObjectMapper::getInstance();
    std::shared_ptr<UiObject> base = mObjMap->getElement(mRequest->elementid());

    if (!base) {
        mResponse->set_status(::aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    } 

    auto found = base->prev();

    if (found != nullptr) {
        UiObject *obj = found.get();
        obj->refresh();
        if (mObjMap->getElement(obj->getId()) == nullptr)
            mObjMap->addElement(std::move(found));

        LOGI("found object : %p elementId:%s", obj, obj->getId().c_str());

        ::aurum::Element *elm = mResponse->mutable_element();
        elm->set_elementid(obj->getId());
        elm->set_package(obj->getApplicationPackage());

        ::aurum::Rect   *rect = elm->mutable_geometry();
        const Rect<int> &size = obj->getScreenBoundingBox();
        rect->set_x(size.mTopLeft.x);
        rect->set_y(size.mTopLeft.y);
        rect->set_width(size.width());
        rect->set_height(size.height());

        ::aurum::Rect   *windowRect = elm->mutable_windowrelativegeometry();
        const Rect<int> &windowRelativeSize = obj->getWindowBoundingBox();
        windowRect->set_x(windowRelativeSize.mTopLeft.x);
        windowRect->set_y(windowRelativeSize.mTopLeft.y);
        windowRect->set_width(windowRelativeSize.width());
        windowRect->set_height(windowRelativeSize.height());

        elm->set_widgettype(obj->getType());
        elm->set_widgetstyle(obj->getElementStyle());

        elm->set_text(obj->getText());
        elm->set_xpath(obj->getXPath());
        elm->set_ocrtext(obj->getOcrText());
        elm->set_automationid(obj->getAutomationId());
        elm->set_package(obj->getApplicationPackage());
        elm->set_role(obj->getRole());

        elm->set_ischecked(obj->isChecked());
        elm->set_ischeckable(obj->isCheckable());
        elm->set_isclickable(obj->isClickable());
        elm->set_isenabled(obj->isEnabled());
        elm->set_isfocused(obj->isFocused());
        elm->set_isfocusable(obj->isFocusable());
        elm->set_isscrollable(obj->isScrollable());
        elm->set_isselected(obj->isSelected());
        elm->set_isshowing(obj->isShowing());
        elm->set_isactive(obj->isActive());
        elm->set_isvisible(obj->isVisible());
        elm->set_isselectable(obj->isSelectable());
        elm->set_ishighlightable(obj->isHighlightable());

        elm->set_minvalue(obj->getMinValue());
        elm->set_maxvalue(obj->getMaxValue());
        elm->set_value(obj->getValue());
        elm->set_increment(obj->getIncrement());

        elm->set_windowangle(obj->getWindowAngle());
        elm->set_targetangle(obj->getTargetAngle());

        elm->set_interface(obj->getInterface());
        elm->set_description(obj->getDescription());

        mResponse->set_status(::aurum::RspStatus::OK);
    } else {
        mResponse->set_status(::aurum::RspStatus::ERROR);
    }

    return grpc::Status::OK;
}
