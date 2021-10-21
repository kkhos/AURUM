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

#ifndef _SEND_KEY_RUNNABLE_H_
#define _SEND_KEY_RUNNABLE_H_

#include <string>

#include "Runnable.h"

/**
 * @class SendKeyRunnable
 *
 * @ingroup aurum
 *
 * @brief Class to send key event and wait a result.
 */
class SendKeyRunnable : public Runnable {
protected:
    std::string mKeycode;

public:
    /**
     * @brief SendKeyRunnable constructor with keycode.
     *
     * @since_tizen 5.5
     */
    SendKeyRunnable(std::string keycode);

    /**
     * @copydoc Runnable::run()
     */
    void run() const override;
};

#endif
