/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd All Rights Reserved
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

#include "AurumServiceImpl.h"

#include <AccessibleWatcher.h>
#include "Commands/Commands.h"
#include "Commands/PostCommand.h"
#include "Commands/PreCommand.h"

#include "config.h"
#include "bootstrap.h"

using namespace grpc;
using namespace aurum;

aurumServiceImpl::aurumServiceImpl()
{
    LOGI("creates watcher instance (start to look up at_spi server)");
    AccessibleWatcher::getInstance();
}

::grpc::Status aurumServiceImpl::execute(Command *cmd, bool clean)
{
    std::unique_ptr<PreCommand>  proxyPreCmd  = std::make_unique<PreCommand>(cmd);
    std::unique_ptr<PostCommand> proxyPostCmd = std::make_unique<PostCommand>(proxyPreCmd.get());
    ::grpc::Status rst = proxyPostCmd->execute();
    if (clean) ObjectMapper::getInstance()->cleanUp();
    return rst;
}

aurumServiceImpl::~aurumServiceImpl()
{
    LOGI("bootstrap service terminated");
}

::grpc::Status aurumServiceImpl::aurumServiceImpl::killServer(
    ::grpc::ServerContext *context, const ::aurum::ReqEmpty *request,
    ::aurum::RspEmpty *response)
{
    std::unique_ptr<KillServerCommand> cmd = std::make_unique<KillServerCommand>(request, response);
    return execute(cmd.get(), false);
}

::grpc::Status aurumServiceImpl::aurumServiceImpl::findElement(
    ::grpc::ServerContext *context, const ::aurum::ReqFindElement *request,
    ::aurum::RspFindElement *response)
{
    std::unique_ptr<FindElementCommand> cmd = std::make_unique<FindElementCommand>(request, response);
    return execute(cmd.get(), true);
}

::grpc::Status aurumServiceImpl::aurumServiceImpl::click(
    ::grpc::ServerContext *context, const ::aurum::ReqClick *request,
    ::aurum::RspClick *response)
{
    std::unique_ptr<ClickCommand> cmd = ClickCommand::createCommand(request, response);
    return execute(cmd.get(), true);
}

::grpc::Status aurumServiceImpl::getValue(::grpc::ServerContext *context,
                                          const ::aurum::ReqGetValue *request,
                                          ::aurum::RspGetValue *response)
{
    std::unique_ptr<GetValueCommand> cmd = std::make_unique<GetValueCommand>(request, response);
    return execute(cmd.get(), true);
}

::grpc::Status aurumServiceImpl::setValue(::grpc::ServerContext *context,
                                          const ::aurum::ReqSetValue *request,
                                          ::aurum::RspSetValue *response)
{
    std::unique_ptr<SetValueCommand> cmd = std::make_unique<SetValueCommand>(request, response);
    return execute(cmd.get(), true);
}

::grpc::Status aurumServiceImpl::getAttribute(
    ::grpc::ServerContext *context, const ::aurum::ReqGetAttribute *request,
    ::aurum::RspGetAttribute *response)
{
    std::unique_ptr<GetAttributeCommand> cmd = GetAttributeCommand::createCommand(request, response);
    return execute(cmd.get(), true);
}

::grpc::Status aurumServiceImpl::getSize(::grpc::ServerContext *context,
                                         const ::aurum::ReqGetSize *request,
                                         ::aurum::RspGetSize *response)
{
    std::unique_ptr<GetSizeCommand> cmd = std::make_unique<GetSizeCommand>(request, response);
    return execute(cmd.get(), true);
}

::grpc::Status aurumServiceImpl::clear(::grpc::ServerContext *context,
                                       const ::aurum::ReqClear *request,
                                       ::aurum::RspClear *response)
{
    std::unique_ptr<ClearCommand> cmd = std::make_unique<ClearCommand>(request, response);
    return execute(cmd.get(), true);
}

::grpc::Status aurumServiceImpl::installApp(
    ::grpc::ServerContext *context,
    ::grpc::ServerReader< ::aurum::ReqInstallApp> *request,
    ::aurum::RspInstallApp *response)
{
    std::unique_ptr<InstallAppCommand> cmd = std::make_unique<InstallAppCommand>(request, response);
    return execute(cmd.get(), false);
}

::grpc::Status aurumServiceImpl::removeApp(::grpc::ServerContext *context,
                                           const ::aurum::ReqRemoveApp *request,
                                           ::aurum::RspRemoveApp *response)
{
    std::unique_ptr<RemoveAppCommand> cmd = std::make_unique<RemoveAppCommand>(request, response);
    return execute(cmd.get(), false);
}

::grpc::Status aurumServiceImpl::getAppInfo(
    ::grpc::ServerContext *context, const ::aurum::ReqGetAppInfo *request,
    ::aurum::RspGetAppInfo *response)
{
    std::unique_ptr<GetAppInfoCommand> cmd = std::make_unique<GetAppInfoCommand>(request, response);
    return execute(cmd.get(), false);
}

::grpc::Status aurumServiceImpl::launchApp(::grpc::ServerContext *context,
                                           const ::aurum::ReqLaunchApp *request,
                                           ::aurum::RspLaunchApp *response)
{
    std::unique_ptr<LaunchAppCommand> cmd = std::make_unique<LaunchAppCommand>(request, response);
    return execute(cmd.get(), false);
}

::grpc::Status aurumServiceImpl::closeApp(::grpc::ServerContext *context,
                                          const ::aurum::ReqCloseApp *request,
                                          ::aurum::RspCloseApp *response)
{
    std::unique_ptr<CloseAppCommand> cmd = std::make_unique<CloseAppCommand>(request, response);
    return execute(cmd.get(), false);
}

::grpc::Status aurumServiceImpl::touchDown(::grpc::ServerContext *context,
                                           const ::aurum::ReqTouchDown *request,
                                           ::aurum::RspTouchDown *response)
{
    std::unique_ptr<TouchDownCommand> cmd = std::make_unique<TouchDownCommand>(request, response);
    return execute(cmd.get(), false);
}

::grpc::Status aurumServiceImpl::touchUp(::grpc::ServerContext *context,
                                         const ::aurum::ReqTouchUp *request,
                                         ::aurum::RspTouchUp *response)
{
    std::unique_ptr<TouchUpCommand> cmd = std::make_unique<TouchUpCommand>(request, response);
    return execute(cmd.get(), false);
}

::grpc::Status aurumServiceImpl::touchMove(::grpc::ServerContext *context,
                                           const ::aurum::ReqTouchMove *request,
                                           ::aurum::RspTouchMove *response)
{
    std::unique_ptr<TouchMoveCommand> cmd = std::make_unique<TouchMoveCommand>(request, response);
    return execute(cmd.get(), false);
}

::grpc::Status aurumServiceImpl::longClick(::grpc::ServerContext *context,
                                           const ::aurum::ReqClick *request,
                                           ::aurum::RspClick * response)
{
    std::unique_ptr<LongClickCommand> cmd = std::make_unique<LongClickCommand>(request, response);
    return execute(cmd.get(), false);
}

::grpc::Status aurumServiceImpl::flick(::grpc::ServerContext *context,
                                       const ::aurum::ReqFlick *request,
                                       ::aurum::RspFlick *response)
{
    std::unique_ptr<FlickCommand> cmd = std::make_unique<FlickCommand>(request, response);
    return execute(cmd.get(), false);
}

::grpc::Status aurumServiceImpl::getDeviceTime(
    ::grpc::ServerContext *context, const ::aurum::ReqGetDeviceTime *request,
    ::aurum::RspGetDeviceTime *response)
{
    std::unique_ptr<GetDeviceTimeCommand> cmd = std::make_unique<GetDeviceTimeCommand>(request, response);
    return execute(cmd.get(), false);
}

::grpc::Status aurumServiceImpl::getLocation(
    ::grpc::ServerContext *context, const ::aurum::ReqGetLocation *request,
    ::aurum::RspGetLocation *response)
{
    std::unique_ptr<GetLocationCommand> cmd = std::make_unique<GetLocationCommand>(request, response);
    return execute(cmd.get(), false);
}

::grpc::Status aurumServiceImpl::sendKey(::grpc::ServerContext *context,
                                         const ::aurum::ReqKey *request,
                                         ::aurum::RspKey *response)
{
    std::unique_ptr<SendKeyCommand> cmd = std::make_unique<SendKeyCommand>(request, response);
    return execute(cmd.get(), false);
}

::grpc::Status aurumServiceImpl::takeScreenshot(::grpc::ServerContext *context,
                                                const ::aurum::ReqTakeScreenshot *request,
                                                ::grpc::ServerWriter< ::aurum::RspTakeScreenshot> *writer)
{
    std::unique_ptr<TakeScreenshotCommand> cmd = std::make_unique<TakeScreenshotCommand>(request, writer);
    return execute(cmd.get(), false);
}

::grpc::Status aurumServiceImpl::dumpObjectTree(::grpc::ServerContext *context,
                           const ::aurum::ReqDumpObjectTree *request,
                           ::aurum::RspDumpObjectTree *response)
{
    std::unique_ptr<DumpObjectTreeCommand> cmd = std::make_unique<DumpObjectTreeCommand>(request, response);
    return execute(cmd.get(), true);
}

::grpc::Status aurumServiceImpl::getScreenSize(::grpc::ServerContext *context,
                                         const ::aurum::ReqGetScreenSize *request,
                                         ::aurum::RspGetScreenSize *response)
{
    std::unique_ptr<GetScreenSizeCommand> cmd = std::make_unique<GetScreenSizeCommand>(request, response);
    return execute(cmd.get(), true);
}

::grpc::Status aurumServiceImpl::actionAndWaitEvent(::grpc::ServerContext *context,
                                         const ::aurum::ReqActionAndWaitEvent *request,
                                         ::aurum::RspActionAndWaitEvent *response)
{
    std::unique_ptr<ActionAndWaitEventCommand> cmd = std::make_unique<ActionAndWaitEventCommand>(request, response);
    return execute(cmd.get(), true);
}

::grpc::Status aurumServiceImpl::setFocus(::grpc::ServerContext *context,
                                         const ::aurum::ReqSetFocus *request,
                                         ::aurum::RspSetFocus *response)
{
    std::unique_ptr<SetFocusCommand> cmd = std::make_unique<SetFocusCommand>(request, response);
    return execute(cmd.get(), true);
}
