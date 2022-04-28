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

    LOGI("Mosquitto onMessage finished");
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

    LOGI("Mosquitto publish data finish");
}