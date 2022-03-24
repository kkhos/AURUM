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

using namespace Aurum;
using namespace AurumInternal;

void on_connect(struct mosquitto *mosq, void *obj, int reason_code)
{
    LOGE("WCC client mosquitto on connect");

    if(reason_code != 0){
        LOGE("WCC client mosquitto  on connect error : %d", reason_code);
        mosquitto_disconnect(mosq);
    }
    int rc;
    rc = mosquitto_subscribe(mosq, NULL, "screen_analyzer/json", 1);
    if(rc != MOSQ_ERR_SUCCESS){
        LOGE("WCC  server on connect subscribe fail");
        mosquitto_disconnect(mosq);
    }

}

void on_publish(struct mosquitto *mosq, void *obj, int mid)
{
    LOGE("WCC client mosquitto on publish");
}

void on_subscribe(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos)
{
    LOGE("WCC server on subscribe qos : %d", qos_count);
}
/*
void PrintJSONValue( const Json::Value &val )
{
    if( val.isString() ) {
        LOGE("WCC string(%s)", val.asString().c_str() ); 
    } else if( val.isBool() ) {
        LOGE("WCC bool(%d)", val.asBool() ); 
    } else if( val.isInt() ) {
        LOGE("WCC int(%d)", val.asInt() ); 
    } else if( val.isUInt() ) {
        LOGE("WCC uint(%u)", val.asUInt() ); 
    } else if( val.isDouble() ) {
        LOGE("WCC double(%f)", val.asDouble() ); 
    }
    else 
    {
        LOGE("WCC unknown type=[%d]", val.type() ); 
    }
}

bool PrintJSONTree( const Json::Value &root, unsigned short depth) 
{
    depth += 1;
    LOGE("WCC {type=[%d], size=%d}", root.type(), root.size() ); 

    if( root.size() > 0 ) {
        LOGE("WCC\n");
        for( Json::Value::const_iterator itr = root.begin() ; itr != root.end() ; itr++ ) {
            // Print depth. 
            for( int tab = 0 ; tab < depth; tab++) {
               LOGE("WCC-"); 
            }
            LOGE("WCC subvalue(");
            PrintJSONValue(itr.key());
            LOGE("WCC) -");
            PrintJSONTree( *itr, depth); 
        }
        return true;
    } else {
        LOGE("WCC ");
        PrintJSONValue(root);
        LOGE("WCC \n" ); 
    }
    return true;
}
*/

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
    //char *_msg = (char*)msg->payload;
    std::string txt((char *)msg->payload);
    LOGE("WCC server on message (%d) (%d) : %s", msg->payloadlen, txt.length(), txt.c_str());

    Json::Reader reader;
    Json::Value root;
    bool ret = reader.parse(txt, root);
    if (!ret) LOGE("WCC parse fail");

    const Json::Value objs = root["objects"];
    LOGE("WCC objects count = %d", root["objectCount"].asInt());    
    LOGE("WCC objects size = %d", objs.size());
    for (int idx = 0; idx < objs.size(); ++idx)
    {
        LOGE("WCC objs[%d] = ID : %d", idx, objs[idx]["id"].asInt());
        LOGE("WCC objs[%d] = TYPE : %s", idx, objs[idx]["type"].asString().c_str());
        LOGE("WCC objs[%d] = GEOMETRY : ", idx);
        const Json::Value geo = objs[idx]["geometry"];
        for (int gidx = 0; gidx < geo.size(); ++gidx) {
            LOGE("WCC objs[%d] = %d", idx, geo[gidx].asInt());
        }
        LOGE("WCC objs[%d] = OCRTEXT : %s", idx, objs[idx]["ocrText"].asString().c_str());
        LOGE("WCC objs[%d] = STATES :", idx);
        const Json::Value sta = objs[idx]["states"];
        for (int sidx = 0; sidx < sta.size(); ++sidx) {
            LOGE("WCC objs[%d] = %s", idx, sta[sidx].asString().c_str());
        }
    }

    //PrintJSONTree(root, 0);
}

ScreenAnalyzerWatcher::ScreenAnalyzerWatcher()
{
    LOGE("WCC Screen Analyzer Constructor");
    mosquitto_lib_init();
    LOGE("WCC mosquitto lib init");
    mosq = mosquitto_new(NULL, true, NULL);
    if(mosq == NULL){
        LOGE("WCC client mosquitto new fail");
        return;
    }

    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_publish_callback_set(mosq, on_publish);
    mosquitto_subscribe_callback_set(mosq, on_subscribe);
    mosquitto_message_callback_set(mosq, on_message);


    int rc;
    rc = mosquitto_connect(mosq, "10.113.16.21", 1883, 60);
    if(rc != MOSQ_ERR_SUCCESS){
        mosquitto_destroy(mosq);
        LOGE("WCC client mosquitto connect fail");
        return;
    }
    rc = mosquitto_loop_start(mosq);
    if(rc != MOSQ_ERR_SUCCESS){
        mosquitto_destroy(mosq);
        LOGE("WCC client mosquitto loop fail");
        return;
    }
    LOGE("WCC Screen Analyzer Constructor finish");
}

ScreenAnalyzerWatcher::~ScreenAnalyzerWatcher()
{
    LOGE("WCC Screen Analyzer destructor");
    mosquitto_disconnect(mosq);
    mosquitto_loop_stop(mosq, false);
    mosquitto_lib_cleanup();
}

void ScreenAnalyzerWatcher::PublishData(std::string path, const Size2D<int> screenSize)
{
    int size = screenSize.width * screenSize.height;
    char buf[size];

    LOGE("WCC Publish Data (%s) size = %d", path.c_str(), size);
   
    std::ifstream ifs(path, std::ifstream::binary);
    
    memset(buf, 0, size);
    ifs.read(buf, size);

    int rc;
    rc = mosquitto_publish(mosq, NULL, "screen_analyzer/image", size, buf, 2, false);
    if(rc != MOSQ_ERR_SUCCESS) {
        LOGE("WCC client mosquitto publish fail");
    }
    LOGE("WCC Publish Finish");
    ifs.close();
}