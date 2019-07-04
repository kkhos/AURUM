#ifndef COMMAND_H
#define COMMAND_H

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "ObjectMapper.h"
#include "aurum.grpc.pb.h"

#include "config.h"

class Command {
public:
    virtual ~Command(){};
    virtual ::grpc::Status execute() = 0;
};

#endif