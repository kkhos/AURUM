#include "GetValueCommand.h"
#include <loguru.hpp>

GetValueCommand::GetValueCommand(const ::aurum::ReqGetValue* request,
                                 ::aurum::RspGetValue*       response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status GetValueCommand::execute()
{
    LOG_SCOPE_F(INFO, "GetValue --------------- ");
    // ObjectMapper *mObjMap = ObjectMapper::getInstance();
    return grpc::Status::OK;
}