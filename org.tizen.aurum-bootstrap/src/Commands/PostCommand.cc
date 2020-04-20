#include "PostCommand.h"
#include <atspi/atspi.h>
#include <loguru.hpp>
#include <Ecore.h>

PostCommand::PostCommand() : PostCommand(nullptr) {}
PostCommand::PostCommand(Command *cmd) : mCommand{cmd} {}

::grpc::Status PostCommand::execute()
{
    ::grpc::Status rst = mCommand->execute();
    LOG_SCOPE_F(INFO, "PostCommand --------------- ");
    mCommand->executePost();
    ecore_main_loop_thread_safe_call_sync([](void *data)->void*{return NULL;}, NULL); //FIXME : extract ecore dep from here
    // do post-command
    return rst;
}

::grpc::Status PostCommand::executePre()
{
    return mCommand->executePre();
}