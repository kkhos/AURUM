#include <iostream>
#include <glib.h>

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "aurum.grpc.pb.h"

#include "AurumServiceImpl.h"
#include "config.h"
#include "loguru.hpp"

using namespace grpc;

static gpointer
_grpc_thread_func (gpointer data __attribute__((unused)))
{
    std::string binding("0.0.0.0:50051");
    aurumServiceImpl service;
    ServerBuilder builder;

    LOG_F(INFO, "[T] Server Listening on %s", binding.c_str());
    builder.AddListeningPort(binding, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    server->Wait();

    return NULL;
}

int main(int argc, char **argv)
{
    const char *logPath = "/tmp/ua.log";
    GMainLoop *loop;

    loguru::init(argc, argv);
    loguru::g_preamble = false;
    loguru::add_file(logPath, loguru::Append, loguru::Verbosity_MAX);
    LOG_SCOPE_F(INFO, "Log : %s", logPath);

    loop = g_main_loop_new ( NULL , FALSE );
    g_thread_new("grpc_thread", _grpc_thread_func, loop);

    g_main_loop_run (loop);
    g_main_loop_unref(loop);

    return 0;
}
