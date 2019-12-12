#include "PreCommand.h"
#include <atspi/atspi.h>
#include <loguru.hpp>

PreCommand::PreCommand() : PreCommand(nullptr) {}
PreCommand::PreCommand(Command *cmd) : mCommand{cmd} {}

::grpc::Status PreCommand::execute()
{
    {
        LOG_SCOPE_F(INFO, "PreCommand --------------- ");
        AtspiAccessible *n = atspi_get_desktop(0);
        free(atspi_accessible_get_name(n, NULL));
        g_object_unref(n);
    }

    return mCommand->execute();
}