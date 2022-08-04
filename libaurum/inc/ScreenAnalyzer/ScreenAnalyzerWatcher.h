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

#ifndef _SCREEN_ANALYZER_WATCHER_H_
#define _SCREEN_ANALYZER_WATCHER_H_

#include <mosquitto.h>

namespace Aurum {

/**
 * @class ScreenAnalyzerWatcher
 *
 * @ingroup aurum
 *
 * @brief ScreenAnalyzerWatcher communicates with Screen Analyzer Server via MQTT Protocol(mosquitto).
 *        It requests screen analyzer to server and parses the result that can be used in aurum.
 */
class ScreenAnalyzerWatcher {
public:

    /**
     * @brief ScreenAnalyzerWatcher constructor.
     *
     * @since_tizen 7.0
     */
    ScreenAnalyzerWatcher();

    /**
     * @brief ScreenAnalyzerWatcher destructor.
     *
     * @since_tizen 7.0
     */
    ~ScreenAnalyzerWatcher();

    /**
     * @brief When message(analyze result) is received from the broker.
     *        parse the data and create SaObject.
     *
     * @param[in] mosq mosquitto instance
     * @param[in] obj user data provided in mosquitto_new
     * @param[in] msg the message data
     *
     * @since_tizen 7.0
     */
    static void onMessage(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg);

    /**
     * @brief Publish current screen image to the broker.
     *        capture current screen and publish a message on a given topic.
     *
     * @since_tizen 7.0
     */
    void PublishData();

    /**
     * @brief Publish current screen image to the broker.
     *        here capture current screen and publish a message on a given topic.
     *
     * @since_tizen 7.0
     */
    std::vector<std::shared_ptr<Aurum::SaObject>> GetSaObjects();

    /**
     * @copydoc PartialMatch::checkCriteria()
     */
    bool checkCriteria(const std::string textA, const std::string textB, const bool textPartialMatch);

    /**
     * @copydoc PartialMatch::checkCriteria()
     */
    bool checkCriteria(const bool boolA, const bool boolB);

    /**
     * @copydoc PartialMatch::checkCriteria()
     */
    bool checkCriteria(const std::shared_ptr<UiSelector> selector, const std::shared_ptr<SaObject> node);

    /**
     * @brief find object from screen analyzer result.
     *
     * @param[in] selector @UiSelector

     *
     * @return SaObject vector if found, else nulltpr
     *
     * @since_tizen 7.0
     */
    std::vector<std::shared_ptr<Aurum::SaObject>> findSaObjects(const std::shared_ptr<UiSelector> selector);

    /**
     * @brief find object from screen analyzer result.
     *
     * @param[in] selector @UiSelector

     *
     * @return SaObject if found, else nulltpr
     *
     * @since_tizen 7.0
     */
    std::shared_ptr<Aurum::SaObject> findSaObject(const std::shared_ptr<UiSelector> selector);

    /**
     * @brief Get focused app name.
     *
     * @param[in] selector @UiSelector
     *
     * @return SaObject if found, else nulltpr
     *
     * @since_tizen 7.0
     */
    std::string GetFocusedAppId();

    /**
     * @brief Set server IP address.
     *
     * @param[in] ip std::string
     *
     * @since_tizen 7.0
     */
    void SetServerIp(std::string ip);

private:
    struct mosquitto *mMosq;
    static std::string mPkgName;
    static std::vector<std::shared_ptr<SaObject>> mSaObjects;
    static bool mLoadDone;
    std::string mIp;
};

}

#endif
