#include "PostCommand.h"
#include <atspi/atspi.h>
#include <loguru.hpp>

PostCommand::PostCommand() {}
PostCommand::PostCommand(Command *cmd) : mCommand{cmd} {}

::grpc::Status PostCommand::execute()
{
    ::grpc::Status rst = mCommand->execute();
    LOG_SCOPE_F(INFO, "PostCommand --------------- ");
    // do post-command
    return rst;
}