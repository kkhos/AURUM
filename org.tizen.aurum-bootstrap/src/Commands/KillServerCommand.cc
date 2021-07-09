#include "bootstrap.h"
#include "KillServerCommand.h"

KillServerCommand::KillServerCommand(const ::aurum::ReqEmpty *request,
                                     ::aurum::RspEmpty *response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status KillServerCommand::execute()
{
    LOGI("Kill Server --------------- ");

    exit(1);

    return grpc::Status::OK;
}