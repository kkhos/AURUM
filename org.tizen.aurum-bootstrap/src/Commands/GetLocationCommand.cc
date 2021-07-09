#include "bootstrap.h"
#include "GetLocationCommand.h"

GetLocationCommand::GetLocationCommand(const ::aurum::ReqGetLocation *request,
                                       ::aurum::RspGetLocation *response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status GetLocationCommand::execute()
{
    LOGI("GetLocation --------------- ");

    mResponse->set_alt(-1);
    mResponse->set_lat(-1);
    mResponse->set_status(::aurum::RspStatus::ERROR);

    return grpc::Status::OK;
}