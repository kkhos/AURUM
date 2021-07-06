#ifndef _BOOTSTRAP_H_
#define _BOOTSTRAP_H_

#include <dlog.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "AURUM-BOOTSTRAP"

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
#define LOGI_RED(fmt, arg...) \
    ({ do { \
        dlog_print(DLOG_ERROR, LOG_TAG, \
                   FONT_RED "%s: %s(%d) > " fmt FONT_RESET, \
                   __FILE__, __func__, __LINE__, ##arg); \
    } while (0); })
#endif

#endif