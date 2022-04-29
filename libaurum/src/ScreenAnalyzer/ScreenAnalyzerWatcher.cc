/*
 * Copyright (c) 2022 Samsung Electronics Co., Ltd All Rights Reserved
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
#include <time.h>
#include <thread>

using namespace Aurum;

std::vector<std::shared_ptr<SaObject>> ScreenAnalyzerWatcher::saObjects;
bool ScreenAnalyzerWatcher::doneLoad;

void onConnect(struct mosquitto *mosq, void *obj, int ret)
{
    LOGI("Mosquitto on connected");

    if(ret != 0) {
        LOGE("Mosquitto  connect error : %d", ret);
        mosquitto_disconnect(mosq);
    }
    int rc;
    rc = mosquitto_subscribe(mosq, NULL, "screen_analyzer/json", 1);
    if(rc != MOSQ_ERR_SUCCESS) {
        LOGE("Mosquitto subscribe fail");
        mosquitto_disconnect(mosq);
    }
}

void onPublish(struct mosquitto *mosq, void *obj, int mid)
{
    LOGI("Mosquitto on published");
}

void onSubscribe(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos)
{
    LOGI("Mosquitto on subscribed. qos count : %d", qos_count);
}

void ScreenAnalyzerWatcher::onMessage(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
    LOGI("Mosquitto onMessage called");
    std::string txt((char *)msg->payload);
    saObjects.clear();

    Json::Reader reader;
    Json::Value root;
    bool ret = reader.parse(txt, root);
    if (!ret) LOGE("Json parse fail");

    const Json::Value objs = root["objects"];

    std::string id;
    std::string type;
    Rect<int> geometry;
    std::string ocrText;
    std::vector<std::string> states{};
    for (int idx = 0; idx < objs.size(); ++idx)
    {
        states.clear();
        id = objs[idx]["id"].asString();

        type = objs[idx]["type"].asString();

        const Json::Value geo = objs[idx]["geometry"];
        geometry = {geo[0].asInt(), geo[1].asInt(), geo[2].asInt(), geo[3].asInt()};

        ocrText = objs[idx]["ocrText"].asString();

        const Json::Value sta = objs[idx]["states"];
        for (int sidx = 0; sidx < sta.size(); ++sidx) {
            states.push_back(sta[sidx].asString());
        }

        saObjects.push_back(std::make_shared<Aurum::SaObject>(id, type, geometry, ocrText, states));
    }

    LOGI("Mosquitto onMessage finished %d objects are initialized",  saObjects.size());

    doneLoad = true;

}

ScreenAnalyzerWatcher::ScreenAnalyzerWatcher()
{
    LOGI("Screen Analyzer Constructor");
    mosquitto_lib_init();
    mosq = mosquitto_new(NULL, true, NULL);
    if(mosq == NULL){
        LOGE("Mosquitto initialize fail");
        return;
    }

    mosquitto_connect_callback_set(mosq, onConnect);
    mosquitto_publish_callback_set(mosq, onPublish);
    mosquitto_subscribe_callback_set(mosq, onSubscribe);
    mosquitto_message_callback_set(mosq, ScreenAnalyzerWatcher::onMessage);

    int rc;
    rc = mosquitto_connect(mosq, serverAddress.c_str(), 1883, 60);
    if(rc != MOSQ_ERR_SUCCESS){
        mosquitto_destroy(mosq);
        LOGE("Mosquitto connect fail");
        return;
    }
    rc = mosquitto_loop_start(mosq);
    if(rc != MOSQ_ERR_SUCCESS){
        mosquitto_destroy(mosq);
        LOGE("Mosquitto loop start fail");
        return;
    }
}

ScreenAnalyzerWatcher::~ScreenAnalyzerWatcher()
{
    LOGI("Screen Analyzer destructor");
    mosquitto_disconnect(mosq);
    mosquitto_loop_stop(mosq, false);
    mosquitto_lib_cleanup();
}

void ScreenAnalyzerWatcher::PublishData(std::string path, const Size2D<int> screenSize)
{
    LOGI("Mosquitto publish data start file path : " path.c_str());
    int size = screenSize.width * screenSize.height;
    char buf[size];

    doneLoad = false;

    std::ifstream ifs(path, std::ifstream::binary);
    memset(buf, 0, size);
    ifs.read(buf, size);
    int rc;
    rc = mosquitto_publish(mosq, NULL, "screen_analyzer/image", size, buf, 2, false);
    if(rc != MOSQ_ERR_SUCCESS) {
        LOGE("Mosquitto publish fail");
    }
    ifs.close();

    while(!doneLoad) {
        std::this_thread::sleep_for(std::chrono::milliseconds{10});
    }

    LOGI("Mosuqitto publish data finish");
}

std::vector<std::shared_ptr<Aurum::SaObject>> ScreenAnalyzerWatcher::GetSaObjects()
{
    return saObjects;
}

void ScreenAnalyzerWatcher::SetJsonText(std::string text)
{
    jsontext = text;
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
        if (selector->mMatchType && checkCriteria(selector->mType, node->getElementType(), 0)) return false;
    }
    if (selector->mMatchClickable && checkCriteria(selector->mIsclickable, node->isClickable())) return false;
    if (selector->mMatchFocused && checkCriteria(selector->mIsfocused, node->isFocused())) return false;
    if (selector->mMatchFocusable && checkCriteria(selector->mIsfocusable, node->isFocusable())) return false;

    return true;
}


std::vector<std::shared_ptr<Aurum::SaObject>> ScreenAnalyzerWatcher::findSaObjects(const std::shared_ptr<UiSelector> selector)
{
    std::vector<std::shared_ptr<Aurum::SaObject>> rets{};

    LOGI("FindSaObject start");
    for (auto saObj : saObjects) {
        if (checkCriteria(selector, saObj)) {
            LOGI("saObj(%s) found", saObj->getId().c_str());
            rets.push_back(saObj);
        }
    }

    return rets;
}
