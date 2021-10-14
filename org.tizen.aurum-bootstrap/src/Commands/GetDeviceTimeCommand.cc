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

#include "bootstrap.h"
#include "GetDeviceTimeCommand.h"

#include <system_settings.h>
#include <utils_i18n.h>
#include <string>
#include "UiDevice.h"

#define BUFSIZE 64

GetDeviceTimeCommand::GetDeviceTimeCommand(
    const ::aurum::ReqGetDeviceTime *request,
    ::aurum::RspGetDeviceTime *response)
    : mRequest{request}, mResponse{response}
{
}

class TizenLocaleTimeConverter {
public:
    static std::string convert(long long timestamp, const char *pattern)
    {
        if (timestamp < 0 || pattern == NULL) return "";

        char *locale, *timezone;

        i18n_udatepg_h pattern_generator = NULL;
        i18n_udate_format_h formatter = NULL;

        i18n_uchar timezone_i18[BUFSIZE+1] = {0,};
        i18n_uchar pattern_i18[BUFSIZE+1]= {0,};
        i18n_uchar best_pattern_i18[BUFSIZE+1]= {0,};
        i18n_uchar result_i18[BUFSIZE+1]= {0,};
        char result[BUFSIZE+1]= {0,};

        int pattern_len, best_pattern_len, result_i18n_len;

        if (SYSTEM_SETTINGS_ERROR_NONE != system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_COUNTRY, &locale))
            return "";

        if (SYSTEM_SETTINGS_ERROR_NONE != system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_TIMEZONE, &timezone)) {
            if (locale) free(locale);
            return "";
        }

        i18n_udatepg_create(locale, &pattern_generator);

        if (!pattern_generator) {
            free (locale);
            free (timezone);
            return "";
        }

        i18n_ustring_copy_ua_n(pattern_i18, pattern, strlen(pattern));
        pattern_len = i18n_ustring_get_length(pattern_i18);

        i18n_udatepg_get_best_pattern(pattern_generator,
                                      pattern_i18, pattern_len,
                                      best_pattern_i18, BUFSIZE, &best_pattern_len);

        i18n_ustring_copy_ua_n(timezone_i18, timezone, strlen(timezone));

        i18n_udatepg_destroy(pattern_generator);

        if (I18N_ERROR_NONE !=
            i18n_udate_create(I18N_UDATE_PATTERN, I18N_UDATE_PATTERN, locale,
                              timezone_i18, -1, best_pattern_i18, -1,
                              &formatter)) {
            free (locale);
            free (timezone);
            return "";
        }

        if (formatter) {
            i18n_udate date = timestamp;
            i18n_udate_format_date(formatter, date, result_i18, BUFSIZE, NULL, &result_i18n_len);
            i18n_ustring_copy_au_n(result , result_i18, BUFSIZE);
            i18n_udate_destroy(formatter);
            return std::string{result};
        }

        free (locale);
        free (timezone);
        return "";
    }
};

::grpc::Status GetDeviceTimeCommand::execute()
{
    LOGI("GetDeviceTime --------------- ");

    std::shared_ptr<UiDevice> obj = UiDevice::getInstance();
    ::aurum::ReqGetDeviceTime_TimeType type = mRequest->type();
    long long                          utcStampMs;

    switch (type) {
    case ::aurum::ReqGetDeviceTime_TimeType::ReqGetDeviceTime_TimeType_WALLCLOCK:
        utcStampMs = obj->getSystemTime(TimeRequestType::WALLCLOCK);
        mResponse->set_localedatetime(
            TizenLocaleTimeConverter::convert(utcStampMs, "EEE, MMM d, yyyy 'at' HH:mm:ss zzz").c_str()
        );
        mResponse->set_timestamputc(utcStampMs);
        break;

    case ::aurum::ReqGetDeviceTime_TimeType::ReqGetDeviceTime_TimeType_SYSTEM:
    default:
        utcStampMs = obj->getSystemTime(TimeRequestType::MONOTONIC);
        mResponse->set_timestamputc(utcStampMs);
        break;
    }

    mResponse->set_status(::aurum::RspStatus::OK);
    return grpc::Status::OK;
}
