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
#include "SaObject.h"
#include <time.h>
#include <thread>
#include <app_manager_extension.h>
#include <capi-video-capture.h>
#include <tdm_helper.h>
#include <tbm_surface.h>
#include <system_info.h>
#include <efl_util.h>

using namespace Aurum;

std::vector<std::shared_ptr<SaObject>> ScreenAnalyzerWatcher::saObjects;
bool ScreenAnalyzerWatcher::doneLoad;
std::string ScreenAnalyzerWatcher::pkgName;
std::vector<unsigned char> YC;

void onConnect(struct mosquitto *mosq, void *obj, int ret)
{
    LOGI("Mosquitto on connected");

    if(ret != 0) {
        LOGI("Mosquitto  connect error : %d", ret);
        mosquitto_disconnect(mosq);
    }
    int rc;
    rc = mosquitto_subscribe(mosq, NULL, "screen_analyzer/json", 1);
    if(rc != MOSQ_ERR_SUCCESS) {
        LOGI("Mosquitto subscribe fail");
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

    //FIXME: Json::Reader has been deprecated. replace it.
    Json::Reader reader;
    Json::Value root;
    bool ret = reader.parse(txt, root);
    if (!ret) LOGI("Json parse fail");

    const Json::Value objs = root["objects"];

    std::string id;
    std::string type;
    Rect<int> geometry;
    std::string ocrText;
    std::vector<std::string> states{};

    id = "9999";
    type = "window";
    geometry = {0, 0, 1920, 1080};
    states.push_back("active");
    ocrText = pkgName;
    saObjects.push_back(std::make_shared<Aurum::SaObject>(id, type, geometry, ocrText, states));

    for (unsigned int idx = 0; idx < objs.size(); ++idx)
    {
        states.clear();
        id = objs[idx]["id"].asString();

        LOGD("objs[%d] = TYPE : %s", idx, objs[idx]["type"].asString().c_str());
        type = objs[idx]["type"].asString();

        LOGD("objs[%d] = GEOMETRY : ", idx);
        const Json::Value geo = objs[idx]["geometry"];
        for (int gidx = 0; gidx < geo.size(); ++gidx) {
            LOGD("objs[%d] = %d", idx, geo[gidx].asInt());
        }
        double x_scale = 2.666666;
        double y_scale = 1.875;

        LOGD("geometry = %d %d %d %d", (int)(geo[0].asInt() * x_scale), (int)(geo[1].asInt() * y_scale), (int)(geo[2].asInt() * x_scale), (int)(geo[3].asInt() * y_scale));
        geometry = { (int)(geo[0].asInt() * x_scale), (int)(geo[1].asInt() * y_scale), (int)(geo[2].asInt() * x_scale), (int)(geo[3].asInt() * y_scale) };

        LOGD("objs[%d] = OCRTEXT : %s", idx, objs[idx]["ocrText"].asString().c_str());
        ocrText = objs[idx]["ocrText"].asString();

        LOGD("objs[%d] = STATES :", idx);
        const Json::Value sta = objs[idx]["states"];
        for (int sidx = 0; sidx < sta.size(); ++sidx) {
            LOGI("objs[%d] = %s", idx, sta[sidx].asString().c_str());
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
        LOGI("Mosquitto initialize fail");
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
        LOGI("Mosquitto connect fail");
        return;
    }
    rc = mosquitto_loop_start(mosq);
    if(rc != MOSQ_ERR_SUCCESS){
        mosquitto_destroy(mosq);
        LOGI("Mosquitto loop start fail");
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

void ScreenAnalyzerWatcher::PublishData()
{
    //FIXME: Creation image API not fixed yet.
    LOGI("PublishData Start");
    doneLoad = false;
    void *ptr = NULL;
    unsigned char *src = NULL;
    unsigned char *dst = NULL;

    efl_util_screenshot_h screenshot = NULL;
    tbm_surface_h tbm_surface = NULL;
    tbm_surface_info_s info;
    const int WIDTH = 1920;
    const int HEIGHT = 1080;

    screenshot = efl_util_screenshot_initialize(WIDTH, HEIGHT);

    if (screenshot)
    {
        tbm_surface = efl_util_screenshot_take_tbm_surface(screenshot);
        if (tbm_surface)
        {
            tbm_surface_map(tbm_surface, TBM_SURF_OPTION_READ, &info);

            LOGI("Image w = %d, h = %d ", info.width, info.height);
            ptr = malloc( WIDTH * HEIGHT * 4 );

            src = (unsigned char *)info.planes[0].ptr;
            dst = (unsigned char *)ptr;
            int src_stride = info.planes[0].stride;
            int dst_stride = info.width * 4;

            memcpy(dst, src, WIDTH * HEIGHT * 4);
            /*
            for (int i = 0; i < HEIGHT; i++)
            {
                memcpy (dst, src, dst_stride);
                src += src_stride;
                dst += dst_stride;
            }
            */
        }
        else
        {
            efl_util_screenshot_deinitialize(screenshot);
            return;
        }

        int payloadlen = WIDTH * HEIGHT * 4;

        LOGI("publish start");
        int rc = mosquitto_publish(mosq, NULL, "screen_analyzer/image_aurum", payloadlen , ptr, 2, false);
        if(rc != MOSQ_ERR_SUCCESS) {
            LOGI("client mosquitto publish fail");
        }

        efl_util_screenshot_deinitialize(screenshot);
    }
    else
    {
        LOGI("Screen shot fail");
        return;
    }

/*
    YC.clear();

    const int WIDTH = 720;
    const int HEIGHT = 576;
    int len = WIDTH * HEIGHT;
    YC.resize(2 * len);

    secvideo_capture_param capture_param;
    capture_param.uYSize = len;
    capture_param.uCSize = len;
    capture_param.pYAddr = (char*)YC.data();
    capture_param.pCAddr = (char*)(YC.data() + len);
    capture_param.ret_width  = 0;
    capture_param.ret_height = 0;
    capture_param.no_lock_no_copy = 0;

	int ret = secvideo_api_capture_screen(WIDTH, HEIGHT, &capture_param);

    // retry with disabling capture protection
	if (ret != 0) {
		system("/usr/bin/capture-tool -d > /dev/null 2>&1");
		ret = secvideo_api_capture_screen(WIDTH, HEIGHT, &capture_param);
	}


    int ret_width = capture_param.ret_width;
	int ret_height = capture_param.ret_height;

	if (HEIGHT != ret_height || WIDTH != ret_width) {
		unsigned char* C = YC.data() + ret_width * ret_height;
		int C_len = ret_width * ret_height / 2;
		for (int i = 0; i < C_len; ++i)
			C[i] = capture_param.pCAddr[i];
	}

    int payloadlen = ret_height  * ret_width / 2 * 3;
    int rc = mosquitto_publish(mosq, NULL, "screen_analyzer/image_aurum", payloadlen , YC.data(), 2, false);
    if(rc != MOSQ_ERR_SUCCESS) {
        LOGI("mosquitto publish fail");
    }
*/
    while(!doneLoad) {
        std::this_thread::sleep_for(std::chrono::milliseconds{10});
    }

    if (tbm_surface) {
	free(ptr);
        tbm_surface_unmap(tbm_surface);
        tbm_surface_destroy(tbm_surface);
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
    for (auto saObj : saObjects) {
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
    for (auto saObj : saObjects) {
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
		LOGI("failed to get app-context");
		return NULL;
	}

	return_val = app_context_get_app_id(app_context, &app_id);
	if (return_val != APP_MANAGER_ERROR_NONE) {
		LOGI("failed to get app_id");
		app_context_destroy(app_context);
		return NULL;
	}

	LOGI("focused app_id = %s", app_id);

	return_val = app_context_destroy(app_context);
	if (return_val != APP_MANAGER_ERROR_NONE) {
        LOGI("failed to destroy app-context");
        return NULL;
    }

    app_context = NULL;

    return app_id;
}
