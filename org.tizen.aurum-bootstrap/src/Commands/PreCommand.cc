#include "bootstrap.h"
#include "PreCommand.h"
#include <atspi/atspi.h>

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
#ifndef TIZEN_TV
    display_state_e state;
    if (device_display_get_state(&state) != DEVICE_ERROR_NONE) {
        LOGE("getting display state has failed");
        return grpc::Status::CANCELLED;
    }

    bool isDisplayOn = DISPLAY_STATE_SCREEN_OFF != state;

    if (!isDisplayOn) {
        LOGI("Display off state try to wake up");
        if (device_power_wakeup(false) != DEVICE_ERROR_NONE) {
            LOGE("turning on display has failed");
            return grpc::Status::CANCELLED;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds{INTV_TURNON_MARGIN});
    }
#endif /* TIZEN_TV */

    mCommand->executePre();
    return mCommand->execute();
}

::grpc::Status PreCommand::executePost()
{
    return mCommand->executePost();
}
