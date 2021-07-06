#include <iostream>
#include <glib.h>
#include <Aurum.h>

#include <service_app.h>
#include <privacy_privilege_manager.h>

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include <aurum.grpc.pb.h>

#include "AurumServiceImpl.h"
#include "config.h"

#define PRIV_MEDIASTORAGE "http://tizen.org/privilege/mediastorage"
#define PRIV_LOCATION "http://tizen.org/privilege/location"

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
    //std::unique_ptr<Server> server(builder.BuildAndStart());
    ctx->server = std::move(builder.BuildAndStart());
    ctx->server->Wait();

    return NULL;
}

static void
reponse_cb(ppm_call_cause_e cause, ppm_request_result_e result,
                                      const char *privilege, void *user_data)
{
    if (cause == PRIVACY_PRIVILEGE_MANAGER_CALL_CAUSE_ERROR) {
        LOGI("PPM Error PRIVACY_PRIVILEGE_MANAGER_CALL_CAUSE_ERROR");
        return;
    }

    switch (result) {
        case PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_ALLOW_FOREVER:
            LOGI("priv:%s PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_ALLOW_FOREVER", privilege);
            break;
        case PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_DENY_FOREVER:
            LOGI("priv:%s PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_ALLOW_FOREVER", privilege);
            break;
        case PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_DENY_ONCE:
            LOGI("priv:%s PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_ALLOW_FOREVER", privilege);
            break;
        default:
            LOGI("priv:%s default", privilege);
            break;
    }
}

static void
check_permission(char *path_privilege)
{
    ppm_check_result_e result;
    int ret;
    LOGI("path_privilege = %s",path_privilege);
    ret = ppm_check_permission(path_privilege, &result);

    if (ret == PRIVACY_PRIVILEGE_MANAGER_ERROR_NONE) {
        switch (result) {
            case PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_ALLOW:
                break;

            case PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_DENY:
                LOGI("PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_DENY");
                ;
                break;

            case PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_ASK:
                LOGI("PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_ASK");
                ppm_request_permission(path_privilege, reponse_cb, NULL);
                break;

            default:
                break;
        }
    } else {
        LOGI("Error to check permission[0x%x]", ret);
    }
}


static bool _service_app_create(void *data)
{
    ServiceContext *ctx = (ServiceContext*)data;

    ctx->loop = g_main_loop_new ( NULL , FALSE );
    ctx->thread = g_thread_new("grpc_thread", _grpc_thread_func, ctx);

    check_permission(PRIV_LOCATION);
    check_permission(PRIV_MEDIASTORAGE);

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
