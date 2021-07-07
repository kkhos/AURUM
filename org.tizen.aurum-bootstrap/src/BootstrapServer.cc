#include <iostream>
#include <glib.h>
#include "bootstrap.h"

#include <service_app.h>

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include <aurum.grpc.pb.h>

#include "AurumServiceImpl.h"
#include "config.h"

using namespace grpc;

typedef struct _ServiceContext {
    GMainLoop *loop;
    GThread   *thread;
    std::unique_ptr<Server> server;
} ServiceContext;

static gpointer
_grpc_thread_func (gpointer data)
{
    ServiceContext *ctx = (ServiceContext*)data;
    std::string binding("0.0.0.0:50051");
    aurumServiceImpl service;
    ServerBuilder builder;

    LOGI("[T] Server Listening on %s", binding.c_str());
    builder.AddListeningPort(binding, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    ctx->server = std::move(builder.BuildAndStart());
    ctx->server->Wait();

    return NULL;
}

static bool _service_app_create(void *data)
{
    ServiceContext *ctx = (ServiceContext*)data;

    ctx->loop = g_main_loop_new ( NULL , FALSE );
    ctx->thread = g_thread_new("grpc_thread", _grpc_thread_func, ctx);

    return true;
}

static void _service_app_terminate(void *data)
{
    ServiceContext *ctx = (ServiceContext*)data;
    ctx->server->Shutdown();
    g_main_loop_unref(ctx->loop);
    g_thread_join(ctx->thread);
}

static void _service_app_control(app_control_h app_control, void *data)
{
    ServiceContext *ctx = (ServiceContext*)data;
}

int main(int argc, char **argv)
{
#ifdef TIZEN_GCOV
    setenv("GCOV_PREFIX", "/tmp/", 1);
#endif

    service_app_lifecycle_callback_s event_callback;
    app_event_handler_h handlers[5] = {NULL, };
    ServiceContext ctx = {0,};

    event_callback.create = _service_app_create;
    event_callback.terminate = _service_app_terminate;
    event_callback.app_control = _service_app_control;

    int result = -1;

    try {
       result = service_app_main(argc, argv, &event_callback, &ctx);
    } catch (const std::exception& e) {
       LOGI("service_app_main exception: %s", e.what());
    }

    return result;
}
