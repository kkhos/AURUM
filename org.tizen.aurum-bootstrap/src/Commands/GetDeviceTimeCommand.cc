#include "GetDeviceTimeCommand.h"

#include <system_settings.h>
#include <utils_i18n.h>

#include <loguru.hpp>

#include "UiDevice.h"

GetDeviceTimeCommand::GetDeviceTimeCommand(
    const ::aurum::ReqGetDeviceTime* request,
    ::aurum::RspGetDeviceTime*       response)
    : mRequest{request}, mResponse{response}
{
}

class TizenLocaleTimeConverter {
public:
    static std::string convert(long long timestamp)
    {
        std::string    time;
        i18n_udatepg_h pattern_generator = NULL;
        char*          locale;

        if (SYSTEM_SETTINGS_ERROR_NONE !=
            system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_COUNTRY,
                                             &locale))
            return time;
        //dlog_print(DLOG_INFO, LOG_TAG, "Current Locale Country : %s\n", locale);
        LOG_F(INFO, "%s", locale);
        i18n_udatepg_create(locale, &pattern_generator);

        if (!pattern_generator) return time;

        i18n_uchar bestPattern[64] = {
            0,
        };
        char bestPatternString[64] = {
            0,
        };
        int         bestPatternLength, len;
        const char* custom_format = "EEE, MMM d, yyyy 'at' HH:mm:ss zzz";
        i18n_uchar  uch_custom_format[64];

        i18n_ustring_copy_ua(uch_custom_format, custom_format);
        len = i18n_ustring_get_length(uch_custom_format);
        i18n_udatepg_get_best_pattern(pattern_generator, uch_custom_format, len,
                                      bestPattern, 64, &bestPatternLength);
        i18n_ustring_copy_au_n(bestPatternString, bestPattern, 64);
        //dlog_print(DLOG_INFO, LOG_TAG, "BestPattern(char[]) : %s \n",                   bestPatternString);
        i18n_udatepg_destroy(pattern_generator);

        i18n_udate_format_h formatter_Current = NULL;
        i18n_uchar          formatted[64] = {
            0,
        };
        char result[64] = {
            0,
        };
        int        formattedLength;
        i18n_udate date;
        char*      timezone_Current;
        i18n_uchar utf16_timezone_Current[64] = {
            0,
        };

        if (SYSTEM_SETTINGS_ERROR_NONE !=
            system_settings_get_value_string(
                SYSTEM_SETTINGS_KEY_LOCALE_TIMEZONE, &timezone_Current))
            return time;

        i18n_ustring_copy_ua_n(utf16_timezone_Current, timezone_Current,
                               strlen(timezone_Current));
        if (I18N_ERROR_NONE !=
            i18n_udate_create(I18N_UDATE_PATTERN, I18N_UDATE_PATTERN, locale,
                              utf16_timezone_Current, -1, bestPattern, -1,
                              &formatter_Current))
            return time;

        if (utf16_timezone_Current) {
            date = (i18n_udate)((double)(timestamp / 1000.0));
            i18n_udate_format_date(formatter_Current, date, formatted, 64, NULL,
                                   &formattedLength);
            i18n_ustring_copy_au_n(result, formatted, 64);
            //dlog_print(DLOG_INFO, LOG_TAG, "Current Date : %s\n", result);
            time = std::string{result};
            LOG_F(INFO, "%s", result);
        }
        i18n_udate_destroy(formatter_Current);
        return time;
    }
};

::grpc::Status GetDeviceTimeCommand::execute()
{
    LOG_SCOPE_F(INFO, "GetDeviceTime --------------- ");

    UiDevice* obj = UiDevice::getInstance(DeviceType::DEFAULT);
    ::aurum::ReqGetDeviceTime_TimeType type = mRequest->type();
    long long                          utcStampMs;

//if (type == ::aurum::ReqGetAttribute_RequestType::ReqGetAttribute_RequestType_VISIBLE)

    switch (type) {
    case ::aurum::ReqGetDeviceTime_TimeType::ReqGetDeviceTime_TimeType_WALLCLOCK:
        utcStampMs = obj->getSystemTime(TypeRequestType::WALLCLOCK);
        mResponse->set_localedatetime(
            TizenLocaleTimeConverter::convert(utcStampMs));
        mResponse->set_timestamputc(utcStampMs);
        break;

    case ::aurum::ReqGetDeviceTime_TimeType::ReqGetDeviceTime_TimeType_SYSTEM:
    default:
        utcStampMs = obj->getSystemTime(TypeRequestType::MONOTONIC);
        mResponse->set_timestamputc(utcStampMs);
        break;
    }

    mResponse->set_status(::aurum::RspStatus::OK);
    return grpc::Status::OK;
}