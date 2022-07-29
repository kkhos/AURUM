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
#include "FindElementCommand.h"
#include "UiObject.h"
#include "UiDevice.h"
#include "UiSelector.h"
#include "Sel.h"
#include "ISearchable.h"
#ifdef MQTT_ENABLED
#include "SaObject.h"
#endif
#include "Until.h"

FindElementCommand::FindElementCommand(const ::aurum::ReqFindElement *request,
                                       ::aurum::RspFindElement *response,
                                       int timeout)
    : mRequest{request}, mResponse{response}, mTimeout(timeout)
{
    mObjMap = ObjectMapper::getInstance();
}
std::shared_ptr<ISearchable> FindElementCommand::getSearchableTop(void)
{
    std::shared_ptr<ISearchable> searchableObj{nullptr};

    if (mRequest->_elementid_case() != 0) {
        searchableObj = mObjMap->getElement(mRequest->elementid());
    }
    if (!searchableObj) searchableObj = UiDevice::getInstance();

    return searchableObj;
}

std::shared_ptr<UiSelector> FindElementCommand::getSelector(void)
{
    auto sel = std::make_shared<UiSelector>();

    if(mRequest->_elementid_case())        sel->id(mRequest->elementid());
    if(mRequest->_automationid_case())     sel->automationid(mRequest->automationid());
    if(mRequest->_textfield_case())        sel->text(mRequest->textfield());
    if(mRequest->_widgettype_case())       sel->type(mRequest->widgettype());
    if(mRequest->_widgetstyle_case())      sel->style(mRequest->widgetstyle());
    if(mRequest->_ischecked_case())        sel->isChecked(mRequest->ischecked());
    if(mRequest->_ischeckable_case())      sel->isCheckable(mRequest->ischeckable());
    if(mRequest->_isclickable_case())      sel->isClickable(mRequest->isclickable());
    if(mRequest->_isenabled_case())        sel->isEnabled(mRequest->isenabled());
    if(mRequest->_isfocused_case())        sel->isFocused(mRequest->isfocused());
    if(mRequest->_isfocusable_case())      sel->isFocusable(mRequest->isfocusable());
    if(mRequest->_isscrollable_case())     sel->isScrollable(mRequest->isscrollable());
    if(mRequest->_isselected_case())       sel->isSelected(mRequest->isselected());
    if(mRequest->_isshowing_case())        sel->isShowing(mRequest->isshowing());
    if(mRequest->_isactive_case())         sel->isActive(mRequest->isactive());
    if(mRequest->_mindepth_case())         sel->minDepth(mRequest->mindepth());
    if(mRequest->_maxdepth_case())         sel->maxDepth(mRequest->maxdepth());
    if(mRequest->_packagename_case())      sel->pkg(mRequest->packagename());
    if(mRequest->_textpartialmatch_case()) sel->textPartialMatch(mRequest->textpartialmatch());
    if(mRequest->_xpath_case())            sel->xpath(mRequest->xpath());
    if(mRequest->_ocrtext_case())          sel->ocrText(mRequest->ocrtext());
    if(mRequest->_geometry_case())         sel->geometry(Rect<int>{mRequest->geometry().x(), mRequest->geometry().y(), mRequest->geometry().x() + mRequest->geometry().width(), mRequest->geometry().y() + mRequest->geometry().height()}, true);

    return sel;
}

::grpc::Status FindElementCommand::execute()
{
    LOGI("findElement --------------- ");
    auto searchableObj = getSearchableTop();
    auto selector     = getSelector();
    std::shared_ptr<UiDevice> mDevice = UiDevice::getInstance();

#ifdef MQTT_ENABLED
    if (mDevice->getExternalAppLaunched())
    {
        mDevice->RequestScreenAnalyze();

        LOGI("Search Object start");

        auto found = mDevice->getSAWatcher()->findSaObject(selector);
        if (found != nullptr) {
             SaObject *obj = found.get();
             ::aurum::Element *elm = mResponse->mutable_element();
             elm->set_elementid(obj->getId());

             ::aurum::Rect *rect = elm->mutable_geometry();
             const Rect<int> &size = obj->getScreenBoundingBox();
             rect->set_x(size.mTopLeft.x);
             rect->set_y(size.mTopLeft.y);
             rect->set_width(size.width());
             rect->set_height(size.height());

             elm->set_widgettype(obj->getType());

             elm->set_text(obj->getOcrText());

             elm->set_isclickable(obj->isClickable());
             elm->set_isfocused(obj->isFocused());
             elm->set_isfocusable(obj->isFocusable());
             elm->set_isactive(obj->isActive());
             elm->set_isshowing(true);
             elm->set_isvisible(true);

             mResponse->set_status(::aurum::RspStatus::OK);
        }
        else
            mResponse->set_status(::aurum::RspStatus::ERROR);
    }
    else
#endif
    {
        if (mDevice->getWithScreenAnalyzer())
        {
            LOGI("Find object with SA information");
            mDevice->RequestScreenAnalyze();
        }

        auto waiter = new Waiter(searchableObj.get(), nullptr, mTimeout);
        auto found = waiter->waitFor(Until::findObject(selector));

        if (found != nullptr) {
            UiObject *obj = found.get();
            obj->refresh();
            if (mObjMap->getElement(obj->getId()) == nullptr)
                mObjMap->addElement(std::move(found));

            LOGI("found object : %p elementId:%s", obj, obj->getId().c_str());

            ::aurum::Element *elm = mResponse->mutable_element();
            elm->set_elementid(obj->getId());
            elm->set_package(obj->getApplicationPackage());

            ::aurum::Rect *rect = elm->mutable_geometry();
            const Rect<int> &size = obj->getScreenBoundingBox();
            rect->set_x(size.mTopLeft.x);
            rect->set_y(size.mTopLeft.y);
            rect->set_width(size.width());
            rect->set_height(size.height());

            ::aurum::Rect *windowRect = elm->mutable_windowrelativegeometry();
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

            elm->set_minvalue(obj->getMinValue());
            elm->set_maxvalue(obj->getMaxValue());
            elm->set_value(obj->getValue());
            elm->set_increment(obj->getIncrement());

            mResponse->set_status(::aurum::RspStatus::OK);
        } else {
            mResponse->set_status(::aurum::RspStatus::ERROR);
        }
    }

    return grpc::Status::OK;
}
