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