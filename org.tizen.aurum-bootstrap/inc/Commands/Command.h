#pragma once

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "ObjectMapper.h"
#include <aurum.grpc.pb.h>

#include "config.h"

class Command {
public:
    virtual ~Command(){};
    virtual ::grpc::Status execute() = 0;
    virtual ::grpc::Status executePost();
    virtual ::grpc::Status executePre();
};