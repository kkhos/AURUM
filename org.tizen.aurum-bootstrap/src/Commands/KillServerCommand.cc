#include "KillServerCommand.h"
#include <loguru.hpp>

KillServerCommand::KillServerCommand(const ::aurum::ReqEmpty* request,
                                     ::aurum::RspEmpty*       response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status KillServerCommand::execute()
{
    LOG_SCOPE_F(INFO, "Kill Server ");

    exit(1);

    return grpc::Status::OK;
}