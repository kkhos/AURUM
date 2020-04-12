#include "Command.h"
#include <loguru.hpp>

::grpc::Status Command::executePost()
{
    return ::grpc::Status::OK;
}

::grpc::Status Command::executePre()
{
    return ::grpc::Status::OK;
}
