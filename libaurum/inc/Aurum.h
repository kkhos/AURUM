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
        dlog_print(DLOG_INFO, LOG_TAG, "%s: %s(%d) > " fmt, \
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

#endif /* _AURUM_H_ */
