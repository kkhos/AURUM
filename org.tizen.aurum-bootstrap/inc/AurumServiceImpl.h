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

#pragma once

#include "bootstrap.h"

class aurumServiceImpl final : public aurum::Bootstrap::Service {
public:
    aurumServiceImpl();
    virtual ~aurumServiceImpl();

protected:
    ::grpc::Status execute(Command *cmd, bool clean);

public:
    ::grpc::Status killServer(::grpc::ServerContext *context,
                              const ::aurum::ReqEmpty *request,
                              ::aurum::RspEmpty *response) override;

    ::grpc::Status findElement(::grpc::ServerContext *context,
                               const ::aurum::ReqFindElement *request,
                               ::aurum::RspFindElement *response) override;

    ::grpc::Status findElements(::grpc::ServerContext *context,
                                const ::aurum::ReqFindElements *request,
                                ::aurum::RspFindElements *response) override;

    ::grpc::Status getValue(::grpc::ServerContext *context,
                            const ::aurum::ReqGetValue *request,
                            ::aurum::RspGetValue *response) override;
    ::grpc::Status setValue(::grpc::ServerContext *context,
                            const ::aurum::ReqSetValue *request,
                            ::aurum::RspSetValue *response) override;
    ::grpc::Status getAttribute(::grpc::ServerContext *context,
                                const ::aurum::ReqGetAttribute *request,
                                ::aurum::RspGetAttribute *response) override;
    ::grpc::Status getSize(::grpc::ServerContext *context,
                           const ::aurum::ReqGetSize *request,
                           ::aurum::RspGetSize *response) override;
    ::grpc::Status clear(::grpc::ServerContext *context,
                         const ::aurum::ReqClear *request,
                         ::aurum::RspClear *response) override;
    ::grpc::Status installApp(
        ::grpc::ServerContext *context,
        ::grpc::ServerReader< ::aurum::ReqInstallApp> *reader,
        ::aurum::RspInstallApp *response) override;
    ::grpc::Status removeApp(::grpc::ServerContext *context,
                             const ::aurum::ReqRemoveApp *request,
                             ::aurum::RspRemoveApp *response) override;
    ::grpc::Status getAppInfo(::grpc::ServerContext *context,
                              const ::aurum::ReqGetAppInfo *request,
                              ::aurum::RspGetAppInfo *response) override;
    ::grpc::Status launchApp(::grpc::ServerContext *context,
                             const ::aurum::ReqLaunchApp *request,
                             ::aurum::RspLaunchApp *response) override;
    ::grpc::Status closeApp(::grpc::ServerContext *context,
                            const ::aurum::ReqCloseApp *request,
                            ::aurum::RspCloseApp *response) override;

    ::grpc::Status click(::grpc::ServerContext *context,
                         const ::aurum::ReqClick *request,
                         ::aurum::RspClick *response) override;
    ::grpc::Status longClick(::grpc::ServerContext *context,
                             const ::aurum::ReqClick *request,
                             ::aurum::RspClick *response) override;
    ::grpc::Status flick(::grpc::ServerContext *context,
                         const ::aurum::ReqFlick *request,
                         ::aurum::RspFlick *response) override;

    ::grpc::Status touchDown(::grpc::ServerContext *context,
                             const ::aurum::ReqTouchDown *request,
                             ::aurum::RspTouchDown *response) override;
    ::grpc::Status touchUp(::grpc::ServerContext *context,
                           const ::aurum::ReqTouchUp *request,
                           ::aurum::RspTouchUp *response) override;
    ::grpc::Status touchMove(::grpc::ServerContext *context,
                             const ::aurum::ReqTouchMove *request,
                             ::aurum::RspTouchMove *response) override;
    ::grpc::Status getDeviceTime(::grpc::ServerContext *context,
                                 const ::aurum::ReqGetDeviceTime *request,
                                 ::aurum::RspGetDeviceTime *response) override;
    ::grpc::Status getLocation(::grpc::ServerContext *context,
                               const ::aurum::ReqGetLocation *request,
                               ::aurum::RspGetLocation *response) override;
    ::grpc::Status sendKey(::grpc::ServerContext *context,
                           const ::aurum::ReqKey *request,
                           ::aurum::RspKey *response) override;

    ::grpc::Status takeScreenshot(::grpc::ServerContext *context,
                            const ::aurum::ReqTakeScreenshot *request,
                            ::grpc::ServerWriter< ::aurum::RspTakeScreenshot> *writer) override;

    ::grpc::Status dumpObjectTree(::grpc::ServerContext *context,
                            const ::aurum::ReqDumpObjectTree *request,
                            ::aurum::RspDumpObjectTree *response) override;
    ::grpc::Status getScreenSize(::grpc::ServerContext *context,
                            const ::aurum::ReqGetScreenSize *request,
                            ::aurum::RspGetScreenSize *response) override;
    ::grpc::Status actionAndWaitEvent(::grpc::ServerContext *context,
                                      const ::aurum::ReqActionAndWaitEvent *request,
                                      ::aurum::RspActionAndWaitEvent *response) override;
    ::grpc::Status setFocus(::grpc::ServerContext *context,
                            const ::aurum::ReqSetFocus *request,
                            ::aurum::RspSetFocus *response) override;
    ::grpc::Status getActiveAppToolkitName(::grpc::ServerContext *context,
                                           const ::aurum::ReqGetActiveAppToolkitName *request,
                                           ::aurum::RspGetActiveAppToolkitName *response) override;
    ::grpc::Status getTextMinBoundingRect(::grpc::ServerContext *context,
                            const ::aurum::ReqGetTextMinBoundingRect *request,
                            ::aurum::RspGetTextMinBoundingRect *response) override;
    ::grpc::Status setTimeout(::grpc::ServerContext *context,
                              const ::aurum::ReqSetTimeout *request,
                              ::aurum::RspSetTimeout *response) override;
    ::grpc::Status setXMLSync(::grpc::ServerContext *context,
                            const ::aurum::ReqSetXMLSync *request,
                            ::aurum::RspSetXMLSync *response) override;
    ::grpc::Status getAngle(::grpc::ServerContext *context,
                            const ::aurum::ReqGetAngle *request,
                            ::aurum::RspGetAngle *response) override;
    ::grpc::Status moveTo(::grpc::ServerContext *context,
                            const ::aurum::ReqMoveTo *request,
                            ::aurum::RspMoveTo *response) override;
    ::grpc::Status next(::grpc::ServerContext *context,
                            const ::aurum::ReqNext *request,
                            ::aurum::RspNext *response) override;
    ::grpc::Status prev(::grpc::ServerContext *context,
                            const ::aurum::ReqPrev *request,
                            ::aurum::RspPrev *response) override;
    ::grpc::Status first(::grpc::ServerContext *context,
                            const ::aurum::ReqFirst *request,
                            ::aurum::RspFirst *response) override;
    ::grpc::Status last(::grpc::ServerContext *context,
                            const ::aurum::ReqLast *request,
                            ::aurum::RspLast *response) override;
    ::grpc::Status getParent(::grpc::ServerContext *context,
                            const ::aurum::ReqGetParent *request,
                            ::aurum::RspGetParent *response) override;
    ::grpc::Status mouseDown(::grpc::ServerContext *context,
                             const ::aurum::ReqMouseDown *request,
                             ::aurum::RspMouseDown *response) override;
    ::grpc::Status mouseUp(::grpc::ServerContext *context,
                           const ::aurum::ReqMouseUp *request,
                           ::aurum::RspMouseUp *response) override;
    ::grpc::Status mouseMove(::grpc::ServerContext *context,
                             const ::aurum::ReqMouseMove *request,
                             ::aurum::RspMouseMove *response) override;
public:
    int WAIT_TIMEOUT_MS;
};
