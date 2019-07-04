#include "LongClickCommand.h"
#include <loguru.hpp>

LongClickCommand::LongClickCommand(const ::aurum::ReqClick* request,
                                   ::aurum::RspClick*       response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status LongClickCommand::execute()
{
    LOG_SCOPE_F(INFO, "LongClick --------------- ");
    // ObjectMapper *mObjMap = ObjectMapper::getInstance();
    return grpc::Status::OK;
}