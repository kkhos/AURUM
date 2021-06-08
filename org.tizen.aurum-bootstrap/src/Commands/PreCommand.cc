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
		display_state_e state;
		if (device_display_get_state(&state) != DEVICE_ERROR_NONE) {
			LOG_F(INFO, "getting display state has failed");
		}

        bool isDisplayOn = DISPLAY_STATE_SCREEN_OFF != state;

		if (device_power_wakeup(false) != DEVICE_ERROR_NONE) {
			LOG_F(INFO, "turning on display has failed");
		}

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
