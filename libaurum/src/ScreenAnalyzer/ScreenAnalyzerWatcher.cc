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

#include "Aurum.h"

#include "ScreenAnalyzerWatcher.h"
#include <fstream>
#include <json/json.h>
#include <sstream>
#include "SaObject.h"
#include <time.h>
#include <thread>
#include <app_manager_extension.h>
#include <tdm_helper.h>
#include <tbm_surface.h>
#include <system_info.h>
#include <efl_util.h>

using namespace Aurum;

//Initialize static member variables
std::vector<std::shared_ptr<SaObject>> ScreenAnalyzerWatcher::mSaObjects;
bool ScreenAnalyzerWatcher::mLoadDone;
std::string ScreenAnalyzerWatcher::mPkgName;

void on_connect(struct mosquitto *mosq, void *obj, int reason_code)
{
    LOGI("mosquitto on connect");

    if(reason_code != 0){
        LOGE("mosquitto  on connect error : %d", reason_code);
        mosquitto_disconnect(mosq);
    }
    int rc;
    rc = mosquitto_subscribe(mosq, NULL, "screen_analyzer/json_aurum", 1);
    if(rc != MOSQ_ERR_SUCCESS){
        LOGE("server on connect subscribe fail");
        mosquitto_disconnect(mosq);
    }

}

void on_publish(struct mosquitto *mosq, void *obj, int mid)
{
    LOGI("mosquitto on publish");
}

void on_subscribe(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos)
{
    LOGI("server on subscribe qos : %d", qos_count);
}

ScreenAnalyzerWatcher::ScreenAnalyzerWatcher()
{
    LOGI("Screen Analyzer constructor");

    mosquitto_lib_init();
    LOGI("mosquitto lib init");
    mMosq = mosquitto_new(NULL, true, NULL);
    if(mMosq == NULL){
        LOGE("mosquitto new fail");
        return;
    }

    mosquitto_connect_callback_set(mMosq, on_connect);
    mosquitto_publish_callback_set(mMosq, on_publish);
    mosquitto_subscribe_callback_set(mMosq, on_subscribe);
    mosquitto_message_callback_set(mMosq, ScreenAnalyzerWatcher::onMessage);

    mIp = "10.113.16.21"; //This is AIM default server user can change this using SetServerIp()

    int rc;
    rc = mosquitto_connect(mMosq, mIp.c_str(), 1883, 60);
    if(rc != MOSQ_ERR_SUCCESS){
        mosquitto_destroy(mMosq);
        LOGE("mosquitto connect fail");
        return;
    }
    rc = mosquitto_loop_start(mMosq);
    if(rc != MOSQ_ERR_SUCCESS){
        mosquitto_destroy(mMosq);
        LOGE("mosquitto loop fail");
        return;
    }
}

ScreenAnalyzerWatcher::~ScreenAnalyzerWatcher()
{
    LOGI("Screen Analyzer destructor");

    if (mMosq)
    {
        mosquitto_disconnect(mMosq);
        mosquitto_loop_stop(mMosq, false);
        mosquitto_lib_cleanup();
        mMosq = nullptr;
    }
}

void ScreenAnalyzerWatcher::onMessage(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
    std::string txt((char *)msg->payload);

    mSaObjects.clear();

    Json::Reader reader;
    Json::Value root;
    bool ret = reader.parse(txt, root);
    if (!ret) LOGE("mosquitto parse fail");

    const Json::Value objs = root["objects"];
    LOGI("objects count = %d", root["objectCount"].asInt());

    std::string id;
    std::string type;
    Rect<int> geometry;
    std::string ocrText;
    std::vector<std::string> states{};

    std::shared_ptr<UiDevice> mDevice = UiDevice::getInstance();
    //Add dummy for window
    id = "9999";
    type = "window";
    geometry = {0, 0, mDevice->getScreenSize().width, mDevice->getScreenSize().height};
    states.push_back("active");
    ocrText = mPkgName;
    mSaObjects.push_back(std::make_shared<Aurum::SaObject>(id, type, geometry, ocrText, states));

    for (unsigned int idx = 0; idx < objs.size(); ++idx)
    {
        states.clear();
        id = objs[idx]["id"].asString();
        LOGI("objs[%d] ID : %s", idx,id.c_str());

        type = objs[idx]["type"].asString();
        LOGI("objs[%d] TYPE : %s", idx, objs[idx]["type"].asString().c_str());

        const Json::Value geo = objs[idx]["geometry"];
        geometry = { geo[0].asInt(), geo[1].asInt(), geo[2].asInt(), geo[3].asInt() };
        LOGI("objs[%d] GEOMETRY : %d %d %d %d", idx, geo[0].asInt(), geo[1].asInt(), geo[2].asInt(), geo[3].asInt());

        ocrText = objs[idx]["ocrText"].asString();
        LOGI("objs[%d] OCRTEXT : %s", idx, objs[idx]["ocrText"].asString().c_str());

        const Json::Value sta = objs[idx]["states"];
        for (unsigned int sidx = 0; sidx < sta.size(); ++sidx) {
            LOGI("objs[%d] STATE: %s", idx, sta[sidx].asString().c_str());
            states.push_back(sta[sidx].asString());
        }

        mSaObjects.push_back(std::make_shared<Aurum::SaObject>(id, type, geometry, ocrText, states));
    }

    mLoadDone = true;
}

void ScreenAnalyzerWatcher::PublishData()
{
    LOGI("Prepare capture data for data publish");
    mLoadDone = false;
    void *ptr = NULL;
    unsigned char *src = NULL;
    unsigned char *dst = NULL;

    efl_util_screenshot_h screenshot = NULL;
    tbm_surface_h tbm_surface = NULL;
    tbm_surface_info_s info;
    std::shared_ptr<UiDevice> mDevice = UiDevice::getInstance();
    const int WIDTH = mDevice->getScreenSize().width;
    const int HEIGHT = mDevice->getScreenSize().height;

    screenshot = efl_util_screenshot_initialize(WIDTH, HEIGHT);

    if (screenshot)
    {
        tbm_surface = efl_util_screenshot_take_tbm_surface(screenshot);
        if (tbm_surface)
        {
            tbm_surface_map(tbm_surface, TBM_SURF_OPTION_READ, &info);

            ptr = malloc( WIDTH * HEIGHT * 4 );
            src = (unsigned char *)info.planes[0].ptr;
            dst = (unsigned char *)ptr;

            memcpy(dst, src, WIDTH * HEIGHT * 4);
        }
        else
        {
            efl_util_screenshot_deinitialize(screenshot);
            return;
        }

        int payloadlen = WIDTH * HEIGHT * 4;

        LOGI("mosquitto publish start");
        int rc = mosquitto_publish(mMosq, NULL, "screen_analyzer/image_aurum", payloadlen , ptr, 2, false);
        if(rc != MOSQ_ERR_SUCCESS) {
            LOGE("mosquitto publish fail");
        }

        tbm_surface_destroy(tbm_surface);
        efl_util_screenshot_deinitialize(screenshot);
    }
    else
    {
        LOGE("Screen shot fail");
        return;
    }

    while(!mLoadDone) {
        std::this_thread::sleep_for(std::chrono::milliseconds{10});
    }

    if (tbm_surface) {
        free(ptr);
        tbm_surface_unmap(tbm_surface);
        tbm_surface_destroy(tbm_surface);
    }
}

std::vector<std::shared_ptr<Aurum::SaObject>> ScreenAnalyzerWatcher::GetSaObjects()
{
    return mSaObjects;
}

bool ScreenAnalyzerWatcher::checkCriteria(const std::string textA, const std::string textB, const bool textPartialMatch)
{
    if (textB.empty()) return true;

    bool rst;
    if (textPartialMatch) {
        if (textB.find(textA) != std::string::npos) rst = false;
        else rst = true;
    }
    else {
        if (!textA.compare(textB)) rst = false;
        else rst = true;
    }

    return rst;
}

bool ScreenAnalyzerWatcher::checkCriteria(const bool boolA, const bool boolB)
{
    return boolA != boolB;
}

bool ScreenAnalyzerWatcher::checkCriteria(const std::shared_ptr<UiSelector> selector,
                                 const std::shared_ptr<Aurum::SaObject> node)
{
    if (selector->mMatchText || selector->mMatchTextPartialMatch) {
        if (selector->mMatchText && checkCriteria(selector->mText, node->getOcrText(), 0)) return false;
        if (selector->mMatchTextPartialMatch && checkCriteria(selector->mTextPartialMatch, node->getOcrText(), 1)) return false;
    }
    if (selector->mMatchId) {
        if (checkCriteria(selector->mId, node->getId(), 0)) return false;
    }
    if (selector->mMatchType) {
        if (selector->mMatchType && checkCriteria(selector->mType, node->getType(), 0)) return false;
    }
    if (selector->mMatchClickable && checkCriteria(selector->mIsclickable, node->isClickable())) return false;
    if (selector->mMatchFocused && checkCriteria(selector->mIsfocused, node->isFocused())) return false;
    if (selector->mMatchFocusable && checkCriteria(selector->mIsfocusable, node->isFocusable())) return false;
    if (selector->mMatchActive && checkCriteria(selector->mIsactive, node->isActive())) return false;
    if (selector->mMatchShowing && checkCriteria(selector->mIsshowing, node->isShowing())) return false;

    return true;
}

std::vector<std::shared_ptr<Aurum::SaObject>> ScreenAnalyzerWatcher::findSaObjects(const std::shared_ptr<UiSelector> selector)
{
    std::vector<std::shared_ptr<Aurum::SaObject>> rets{};

    LOGI("FindSaObject start");
    for (auto saObj : mSaObjects) {
        if (checkCriteria(selector, saObj)) {
            LOGI("saObj(%s) pushed", saObj->getId().c_str());
            rets.push_back(saObj);
        }
    }

    return rets;
}

std::shared_ptr<Aurum::SaObject> ScreenAnalyzerWatcher::findSaObject(const std::shared_ptr<UiSelector> selector)
{
    LOGI("FindSaObject start");
    for (auto saObj : mSaObjects) {
        if (checkCriteria(selector, saObj)) {
            LOGI("saObj(%s) pushed", saObj->getId().c_str());
            return saObj;
        }
    }

    return nullptr;
}

std::string ScreenAnalyzerWatcher::GetFocusedAppId()
{
    int return_val = APP_MANAGER_ERROR_NONE;

    app_context_h app_context;
    char *app_id = NULL;

    return_val  = app_manager_get_focused_app_context(&app_context);
    if (return_val != APP_MANAGER_ERROR_NONE) {
        LOGE("failed to get app-context");
        return NULL;
    }

    return_val = app_context_get_app_id(app_context, &app_id);
    if (return_val != APP_MANAGER_ERROR_NONE) {
        LOGE("failed to get app_id");
        app_context_destroy(app_context);
        return NULL;
    }

    LOGI("focused app_id = %s", app_id);

    return_val = app_context_destroy(app_context);
    if (return_val != APP_MANAGER_ERROR_NONE) {
        LOGE("failed to destroy app-context");
        return NULL;
    }

    app_context = NULL;

    return app_id;
}

void ScreenAnalyzerWatcher::SetServerIp(std::string ip)
{
    LOGI("SA Server Initialize");
    mIp = ip;

    if (mMosq)
    {
        mosquitto_disconnect(mMosq);
        mosquitto_loop_stop(mMosq, false);
        mosquitto_lib_cleanup();
        mMosq = nullptr;
    }

    mosquitto_lib_init();
    LOGI("mosquitto lib init");
    mMosq = mosquitto_new(NULL, true, NULL);
    if(mMosq == NULL){
        LOGE("mosquitto new fail");
        return;
    }

    mosquitto_connect_callback_set(mMosq, on_connect);
    mosquitto_publish_callback_set(mMosq, on_publish);
    mosquitto_subscribe_callback_set(mMosq, on_subscribe);
    mosquitto_message_callback_set(mMosq, ScreenAnalyzerWatcher::onMessage);

    int rc;
    rc = mosquitto_connect(mMosq, mIp.c_str(), 1883, 60);
    if (rc != MOSQ_ERR_SUCCESS){
        mosquitto_destroy(mMosq);
        LOGE("mosquitto connect fail");
        return;
    }
    rc = mosquitto_loop_start(mMosq);
    if (rc != MOSQ_ERR_SUCCESS){
        mosquitto_destroy(mMosq);
        LOGE("mosquitto loop fail");
        return;
    }
}
