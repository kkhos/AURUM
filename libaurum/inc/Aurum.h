#pragma once

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
#include "Rect.h"
#include "PartialMatch.h"
#include "Comparer.h"
#include "A11yEvent.h"
#include "Runnables.h"
