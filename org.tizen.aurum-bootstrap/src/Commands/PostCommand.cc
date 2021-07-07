#include "bootstrap.h"
#include "PostCommand.h"
#include <atspi/atspi.h>

PostCommand::PostCommand() : PostCommand(nullptr) {}
PostCommand::PostCommand(Command *cmd) : mCommand{cmd} {}

::grpc::Status PostCommand::execute()
{
    ::grpc::Status rst = mCommand->execute();

    mCommand->executePost();

    return rst;
}

::grpc::Status PostCommand::executePre()
{
    return mCommand->executePre();
}