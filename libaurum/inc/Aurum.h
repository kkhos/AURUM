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

#ifndef _AURUM_H_
#define _AURUM_H_

#include <dlog.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "AURUM"

#ifndef LOGI
#define LOGI(fmt, arg...) \
    ({ do { \
        dlog_print(DLOG_INFO, LOG_TAG, \
                   "%s: %s(%d) > " fmt, __FILE__, __func__, __LINE__, ##arg); \
    } while (0); })
#endif

#ifndef LOGE
#define FONT_RED    "\033[31m"
#define FONT_RESET  "\033[0m"
#define LOGE(fmt, arg...) \
    ({ do { \
        dlog_print(DLOG_ERROR, LOG_TAG, \
                   FONT_RED "%s: %s(%d) > " fmt FONT_RESET, \
                   __FILE__, __func__, __LINE__, ##arg); \
    } while (0); })
#endif

#include "UiDevice.h"
#include "UiObject.h"
#include "UiSelector.h"
#include "Sel.h"
#include "Until.h"
#include "Waiter.h"
#include "ISearchable.h"
#include "IDevice.h"
#include "Accessible.h"
#include "Point2D.h"
#include "Size2D.h"
#include "Rect.h"
#include "PartialMatch.h"
#include "Comparer.h"
#include "A11yEvent.h"
#include "Runnables.h"

#endif
