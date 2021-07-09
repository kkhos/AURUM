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
    mCommand->executePre();
    return mCommand->execute();
}

::grpc::Status PreCommand::executePost()
{
    return mCommand->executePost();
}
