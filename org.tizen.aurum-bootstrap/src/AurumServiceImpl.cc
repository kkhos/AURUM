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

#include "AurumServiceImpl.h"
#include "AccessibleWatcher.h"
#include "Commands/Commands.h"
#include "Commands/PostCommand.h"
#include "Commands/PreCommand.h"

using namespace grpc;
using namespace aurum;

aurumServiceImpl::aurumServiceImpl()
    : WAIT_TIMEOUT_MS{0}
{
    LOGI("creates watcher instance (start to look up at_spi server)");
    AccessibleWatcher::getInstance();
}

::grpc::Status aurumServiceImpl::execute(Command *cmd, bool clean)
{
    std::unique_ptr<PreCommand>  proxyPreCmd  = std::make_unique<PreCommand>(cmd);
    std::unique_ptr<PostCommand> proxyPostCmd = std::make_unique<PostCommand>(proxyPreCmd.get());
    ::grpc::Status ret = proxyPostCmd->execute();
    if (clean) ObjectMapper::getInstance()->cleanUp();
    return ret;
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
    std::unique_ptr<FindElementCommand> cmd = std::make_unique<FindElementCommand>(request, response, WAIT_TIMEOUT_MS);
    return execute(cmd.get(), true);
}

::grpc::Status aurumServiceImpl::aurumServiceImpl::findElements(
    ::grpc::ServerContext *context, const ::aurum::ReqFindElements *request,
    ::aurum::RspFindElements *response)
{
    std::unique_ptr<FindElementsCommand> cmd = std::make_unique<FindElementsCommand>(request, response, WAIT_TIMEOUT_MS);
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

::grpc::Status aurumServiceImpl::getActiveAppToolkitName(::grpc::ServerContext *context,
                                                         const ::aurum::ReqGetActiveAppToolkitName *request,
                                                         ::aurum::RspGetActiveAppToolkitName *response)
{
    std::unique_ptr<GetActiveAppToolkitNameCommand> cmd = std::make_unique<GetActiveAppToolkitNameCommand>(request, response);
    return execute(cmd.get(), true);
}

::grpc::Status aurumServiceImpl::getTextMinBoundingRect(::grpc::ServerContext *context,
                                         const ::aurum::ReqGetTextMinBoundingRect *request,
                                         ::aurum::RspGetTextMinBoundingRect *response)
{
    std::unique_ptr<GetTextMinBoundingRectCommand> cmd = std::make_unique<GetTextMinBoundingRectCommand>(request, response);
    return execute(cmd.get(), true);
}

::grpc::Status aurumServiceImpl::setTimeout(::grpc::ServerContext *context,
                                            const ::aurum::ReqSetTimeout *request,
                                            ::aurum::RspSetTimeout *response)
{
    WAIT_TIMEOUT_MS = request->timeout();
    LOGI("setTimeout: %d", WAIT_TIMEOUT_MS);

    response->set_status(::aurum::RspStatus::OK);
    return ::grpc::Status::OK;
}

::grpc::Status aurumServiceImpl::setXMLSync(::grpc::ServerContext *context,
                                            const ::aurum::ReqSetXMLSync *request,
                                            ::aurum::RspSetXMLSync *response)
{
    std::unique_ptr<SetXMLSyncCommand> cmd = std::make_unique<SetXMLSyncCommand>(request, response);
    return execute(cmd.get(), true);
}

::grpc::Status aurumServiceImpl::getAngle(::grpc::ServerContext *context,
                                          const ::aurum::ReqGetAngle *request,
                                          ::aurum::RspGetAngle *response)
{
    std::unique_ptr<GetAngleCommand> cmd = std::make_unique<GetAngleCommand>(request, response);
    return execute(cmd.get(), true);
}

::grpc::Status aurumServiceImpl::moveTo(::grpc::ServerContext *context,
                                         const ::aurum::ReqMoveTo *request,
                                         ::aurum::RspMoveTo *response)
{
    std::unique_ptr<MoveToCommand> cmd = std::make_unique<MoveToCommand>(request, response);
    return execute(cmd.get(), true);
}

::grpc::Status aurumServiceImpl::next(::grpc::ServerContext *context,
                                         const ::aurum::ReqNext *request,
                                         ::aurum::RspNext *response)
{
    std::unique_ptr<NextCommand> cmd = std::make_unique<NextCommand>(request, response);
    return execute(cmd.get(), true);
}

::grpc::Status aurumServiceImpl::prev(::grpc::ServerContext *context,
                                         const ::aurum::ReqPrev *request,
                                         ::aurum::RspPrev *response)
{
    std::unique_ptr<PrevCommand> cmd = std::make_unique<PrevCommand>(request, response);
    return execute(cmd.get(), true);
}

::grpc::Status aurumServiceImpl::first(::grpc::ServerContext *context,
                                         const ::aurum::ReqFirst *request,
                                         ::aurum::RspFirst *response)
{
    std::unique_ptr<FirstCommand> cmd = std::make_unique<FirstCommand>(request, response);
    return execute(cmd.get(), true);
}
::grpc::Status aurumServiceImpl::last(::grpc::ServerContext *context,
                                         const ::aurum::ReqLast *request,
                                         ::aurum::RspLast *response)
{
    std::unique_ptr<LastCommand> cmd = std::make_unique<LastCommand>(request, response);
    return execute(cmd.get(), true);
}

::grpc::Status aurumServiceImpl::getParent(::grpc::ServerContext *context,
                                         const ::aurum::ReqGetParent *request,
                                         ::aurum::RspGetParent *response)
{
    std::unique_ptr<GetParentCommand> cmd = std::make_unique<GetParentCommand>(request, response);
    return execute(cmd.get(), true);
}

::grpc::Status aurumServiceImpl::mouseDown(::grpc::ServerContext *context,
                                           const ::aurum::ReqMouseDown *request,
                                           ::aurum::RspMouseDown *response)
{
    std::unique_ptr<MouseDownCommand> cmd = std::make_unique<MouseDownCommand>(request, response);
    return execute(cmd.get(), false);
}

::grpc::Status aurumServiceImpl::mouseUp(::grpc::ServerContext *context,
                                         const ::aurum::ReqMouseUp *request,
                                         ::aurum::RspMouseUp *response)
{
    std::unique_ptr<MouseUpCommand> cmd = std::make_unique<MouseUpCommand>(request, response);
    return execute(cmd.get(), false);
}

::grpc::Status aurumServiceImpl::mouseMove(::grpc::ServerContext *context,
                                           const ::aurum::ReqMouseMove *request,
                                           ::aurum::RspMouseMove *response)
{
    std::unique_ptr<MouseMoveCommand> cmd = std::make_unique<MouseMoveCommand>(request, response);
    return execute(cmd.get(), false);
}

::grpc::Status aurumServiceImpl::setIncludeHidden(::grpc::ServerContext *context,
                                                  const ::aurum::ReqSetIncludeHidden *request,
                                                  ::aurum::RspSetIncludeHidden *response)
{
    std::unique_ptr<SetIncludeHiddenCommand> cmd = std::make_unique<SetIncludeHiddenCommand>(request, response);
    return execute(cmd.get(), false);
}

::grpc::Status aurumServiceImpl::getIncludeHidden(::grpc::ServerContext *context,
                                                  const ::aurum::ReqGetIncludeHidden *request,
                                                  ::aurum::RspGetIncludeHidden *response)
{
    std::unique_ptr<GetIncludeHiddenCommand> cmd = std::make_unique<GetIncludeHiddenCommand>(request, response);
    return execute(cmd.get(), false);
}

::grpc::Status aurumServiceImpl::addWatcher(::grpc::ServerContext *context,
                                            const ::aurum::ReqAddWatcher *request,
                                            ::aurum::RspAddWatcher *response)
{
    std::unique_ptr<AddWatcherCommand> cmd = std::make_unique<AddWatcherCommand>(request, response);
    return execute(cmd.get(), false);
}

::grpc::Status aurumServiceImpl::clearWatcher(::grpc::ServerContext *context,
                                            const ::aurum::ReqClearWatcher *request,
                                            ::aurum::RspClearWatcher *response)
{
    std::unique_ptr<ClearWatcherCommand> cmd = std::make_unique<ClearWatcherCommand>(request, response);
    return execute(cmd.get(), false);
}
