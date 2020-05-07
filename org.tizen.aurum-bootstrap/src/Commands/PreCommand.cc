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

        bool isDisplayOn = DISPLAY_STATE_SCREEN_OFF != (display_state_e)((int)ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
            display_state_e state;
            if(device_display_get_state(&state) == DEVICE_ERROR_NONE) {
                return (void*)(state);
            }
            LOG_F(INFO, "getting display state has failed");
            return NULL;
        }, NULL));

        ecore_main_loop_thread_safe_call_sync([](void *data)->void*{
            if (device_power_wakeup(false) != DEVICE_ERROR_NONE) {
                LOG_F(INFO, "turning on display has failed");
                return NULL;
            }
            return NULL;
        }, NULL);

        if (!isDisplayOn)
            std::this_thread::sleep_for(std::chrono::milliseconds{INTV_TURNON_MARGIN});
    }
    mCommand->executePre();
    return mCommand->execute();
}

::grpc::Status PreCommand::executePost()
{
    return mCommand->executePost();
}
