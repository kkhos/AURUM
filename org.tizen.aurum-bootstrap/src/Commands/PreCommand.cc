#include "PreCommand.h"
#include <atspi/atspi.h>
#include <loguru.hpp>

#include <thread>
#include <chrono>

#include <Ecore.h>
#include <device/power.h>
#include <device/display.h>
#include <device/callback.h>

PreCommand::PreCommand() : PreCommand(nullptr) {}
PreCommand::PreCommand(Command *cmd) : mCommand{cmd} {}

::grpc::Status PreCommand::execute()
{
    {
        LOG_SCOPE_F(INFO, "PreCommand --------------- ");

        bool isDisplayOn = (bool)ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
            display_state_e state;
            if(device_display_get_state(&state) == DEVICE_ERROR_NONE) {
                return (void*)(state);
            }
            return NULL;
        }, NULL) != (display_state_e)DISPLAY_STATE_SCREEN_OFF;

        if (!isDisplayOn)
        {
            LOG_F(INFO, "display state : SCREEN_OFF, turn display on");
            ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
                if (device_power_wakeup(false) != DEVICE_ERROR_NONE) {
                    LOG_F(INFO, "turning display up has failed");
                    return NULL;
                }
                if (device_power_request_lock(POWER_LOCK_DISPLAY, 10*1000) != DEVICE_ERROR_NONE) {
                    LOG_F(INFO, "request lock device has failed");
                    return NULL;
                }
                return NULL;
            }, NULL); //FIXME : extract ecore dep from here

            // sleep for display on effect
            std::this_thread::sleep_for(std::chrono::milliseconds{5*1000});
        } else {
            LOG_F(INFO, "display state : SCREEN_ON, lock display on");
            ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
                if (device_power_request_lock(POWER_LOCK_DISPLAY, 10*1000) != DEVICE_ERROR_NONE) {
                    LOG_F(INFO, "request lock device has failed");
                    return NULL;
                }
                return NULL;
            }, NULL); //FIXME : extract ecore dep from here
        }

        AtspiAccessible *n = atspi_get_desktop(0);
        if (n) {
            char *name = atspi_accessible_get_name(n, NULL);
            if(name) free(name);
            g_object_unref(n);
        }
    }
    mCommand->executePre();
    return mCommand->execute();
}

::grpc::Status PreCommand::executePost()
{
    return mCommand->executePost();
}
