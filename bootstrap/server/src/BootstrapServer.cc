#include <iostream>

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "aurum.grpc.pb.h"

#include "AurumServiceImpl.h"
#include "config.h"
#include "loguru.hpp"

using namespace grpc;

int main(int argc, char **argv)
{
    const char *logPath = "/tmp/ua.log";
    loguru::init(argc, argv);
    loguru::g_preamble = false;
    loguru::add_file(logPath, loguru::Append, loguru::Verbosity_MAX);
    LOG_SCOPE_F(INFO, "Log : %s", logPath);

    std::string binding("0.0.0.0:50051");
    aurumServiceImpl service;
    ServerBuilder builder;

    LOG_F(INFO, "Server Listening on %s", binding.c_str());
    builder.AddListeningPort(binding, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    server->Wait();

    return 0;
}
