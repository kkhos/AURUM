#include "GetSizeCommand.h"
#include <loguru.hpp>

GetSizeCommand::GetSizeCommand(const ::aurum::ReqGetSize* request,
                               ::aurum::RspGetSize*       response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status GetSizeCommand::execute()
{
    LOG_SCOPE_F(INFO, "GetSize --------------- ");
    // ObjectMapper *mObjMap = ObjectMapper::getInstance();
    return grpc::Status::OK;
}