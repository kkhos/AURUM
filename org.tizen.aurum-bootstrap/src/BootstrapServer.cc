/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *               http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#include <iostream>
#include <service_app.h>
#include <glib.h>
#include <vconf.h>
#include <system_info.h>
#include <bundle_internal.h>

#include "AurumServiceImpl.h"
#include "config.h"

using namespace grpc;

typedef struct _ServiceContext {
    GThread *thread;
    std::unique_ptr<Server> server;
    bool forceTouchEnabled;
    int port;
} ServiceContext;

static void _vconf_force_enable_touch_set(void *data, bool enable)
{
    ServiceContext *ctx = (ServiceContext *)data;
    int ret;

    if (ctx->forceTouchEnabled == enable) return;

    ret = vconf_set_bool("memory/window_system/input/force_enable_touch", enable);
    if (ret != VCONF_OK) {
        LOGE("Fail to set touch enable via vconf");
        return;
    }

    ctx->forceTouchEnabled = enable;
}

static gpointer
_grpc_thread_func(gpointer data)
{
    ServiceContext *ctx = (ServiceContext *)data;
    std::string binding("0.0.0.0:");
    binding.append(std::to_string(ctx->port));
    aurumServiceImpl service;
    ServerBuilder builder;
    char *value;
    int ret;

    ctx->forceTouchEnabled = false;
    ret = system_info_get_platform_string("http://tizen.org/feature/profile", &value);
    if (ret != SYSTEM_INFO_ERROR_NONE) LOGE("Fail to get system profile infomation");
    else {
        if (!strncmp("tv", value, 2))
            _vconf_force_enable_touch_set(ctx, true);

        free(value);
    }

    LOGI("Server Listening on %s", binding.c_str());
    builder.AddListeningPort(binding, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    ctx->server = std::move(builder.BuildAndStart());
    ctx->server->Wait();

    return NULL;
}

static bool _service_app_create(void *data)
{
    ServiceContext *ctx = (ServiceContext *)data;

    ctx->thread = g_thread_new("grpc_thread", _grpc_thread_func, ctx);

    if (!ctx->thread) {
        LOGE("Thread is empty!");
        return false;
    }

    return true;
}

static void _service_app_terminate(void *data)
{
    ServiceContext *ctx = (ServiceContext *)data;

    if (ctx->forceTouchEnabled)
        _vconf_force_enable_touch_set(ctx, false);
    ctx->server->Shutdown();
    g_thread_join(ctx->thread);
}

static void _service_app_control(app_control_h app_control, void *data)
{
    ServiceContext *ctx = (ServiceContext *)data;
    if (!ctx)
        LOGE("Service context is empty!");
}

int main(int argc, char **argv)
{
#ifdef TIZEN_GCOV
    setenv("GCOV_PREFIX", "/tmp/", 1);
#endif

    service_app_lifecycle_callback_s event_callback;
    ServiceContext ctx = {0,};

    event_callback.create = _service_app_create;
    event_callback.terminate = _service_app_terminate;
    event_callback.app_control = _service_app_control;

    int result = -1;

    bundle *b  = bundle_import_from_argv(argc, argv);
    char *port = NULL;
    bundle_get_str (b, "port",  &port);
    if (!port) {
        ctx.port = 50051;
    }
    else {
	ctx.port = atoi(port);
	if (ctx.port < 50051 || ctx.port > 50061) {
	    LOGE("Port number is invalid!");
	    ctx.port = 50051;
	}
    }
    bundle_free(b);

    try {
       result = service_app_main(argc, argv, &event_callback, &ctx);
    } catch (const std::exception& e) {
       LOGI("service_app_main exception: %s", e.what());
    }

    return result;
}
