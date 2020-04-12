#include "GetDeviceTimeCommand.h"
#include <loguru.hpp>

GetDeviceTimeCommand::GetDeviceTimeCommand(
    const ::aurum::ReqGetDeviceTime* request,
    ::aurum::RspGetDeviceTime*       response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status GetDeviceTimeCommand::execute()
{
    LOG_SCOPE_F(INFO, "GetDeviceTime --------------- ");
    // ObjectMapper *mObjMap = ObjectMapper::getInstance();
    return grpc::Status::OK;
}