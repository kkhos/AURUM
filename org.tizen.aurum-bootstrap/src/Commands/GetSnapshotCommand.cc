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

#include "GetSnapshotCommand.h"

#include "ObjectMapper.h"
#include "UiDevice.h"
#include "UiObject.h"

#include <cstdint>
#include <unordered_set>

namespace {

void fillElement(::aurum::Element *elm, UiObject *obj, const std::string &elementId)
{
    elm->set_elementid(elementId);
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
    elm->set_ishighlighted(obj->isHighlighted());

    elm->set_minvalue(obj->getMinValue());
    elm->set_maxvalue(obj->getMaxValue());
    elm->set_value(obj->getValue());
    elm->set_increment(obj->getIncrement());
    elm->set_valuetext(obj->getValueText());

    elm->set_windowangle(obj->getWindowAngle());
    elm->set_targetangle(obj->getTargetAngle());

    elm->set_interface(obj->getInterface());
    elm->set_description(obj->getDescription());
    elm->set_imgsrc(obj->getImgSrc());
}

} // namespace

GetSnapshotCommand::GetSnapshotCommand(const ::aurum::ReqGetSnapshot *request,
                                       ::aurum::RspGetSnapshot *response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status GetSnapshotCommand::execute()
{
    LOGI("GetSnapshot --------------- ");
    (void)mRequest;

    ObjectMapper *mObjMap = ObjectMapper::getInstance();
    auto device = UiDevice::getInstance();

    auto rootNodes = device->getWindowRoot();
    if (rootNodes.empty()) {
        mResponse->set_status(::aurum::RspStatus::ERROR);
        return grpc::Status::OK;
    }

    std::unordered_set<uintptr_t> dedupNodes{};
    int snapshotIndex = 0;

    for (const auto &rootNode : rootNodes) {
        if (!rootNode) continue;

        auto rootObj = std::make_shared<UiObject>(device, nullptr, rootNode);
        auto snapshot = rootObj->getSnapshot();

        for (const auto &entry : snapshot) {
            auto snapshotObj = entry.second;
            if (!snapshotObj) continue;

            auto rawNode = reinterpret_cast<uintptr_t>(snapshotObj->getAccessibleNode().get());
            if (!dedupNodes.insert(rawNode).second) continue;

            snapshotObj->refresh();
            auto snapshotId = std::string("e") + std::to_string(++snapshotIndex);
            mObjMap->setElement(snapshotId, snapshotObj);

            auto *elm = mResponse->add_elements();
            fillElement(elm, snapshotObj.get(), snapshotId);
        }
    }

    mResponse->set_status(snapshotIndex == 0 ? ::aurum::RspStatus::ERROR : ::aurum::RspStatus::OK);
    return grpc::Status::OK;
}
