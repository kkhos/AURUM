#include "GetLocationCommand.h"
#include <loguru.hpp>

GetLocationCommand::GetLocationCommand(const ::aurum::ReqGetLocation* request,
                                       ::aurum::RspGetLocation*       response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status GetLocationCommand::execute()
{
    LOG_SCOPE_F(INFO, "CliGetLocation --------------- ");
    // ObjectMapper *mObjMap = ObjectMapper::getInstance();
    return grpc::Status::OK;
}