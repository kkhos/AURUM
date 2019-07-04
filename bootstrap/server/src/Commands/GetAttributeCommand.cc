#include "GetAttributeCommand.h"
#include <loguru.hpp>

GetAttributeCommand::GetAttributeCommand(
    const ::aurum::ReqGetAttribute* request, ::aurum::RspGetAttribute* response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status GetAttributeCommand::execute()
{
    LOG_SCOPE_F(INFO, "GetAttribute --------------- ");
    // ObjectMapper *mObjMap = ObjectMapper::getInstance();
    return grpc::Status::OK;
}