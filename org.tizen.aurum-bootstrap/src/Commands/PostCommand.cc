#include "PostCommand.h"
#include <atspi/atspi.h>
#include <loguru.hpp>

PostCommand::PostCommand() : PostCommand(nullptr) {}
PostCommand::PostCommand(Command *cmd) : mCommand{cmd} {}

::grpc::Status PostCommand::execute()
{
    ::grpc::Status rst = mCommand->execute();
    LOG_SCOPE_F(INFO, "PostCommand --------------- ");
    mCommand->executePost();
    // do post-command
    return rst;
}

::grpc::Status PostCommand::executePre()
{
    return mCommand->executePre();
}