#include "AurumServiceImpl.h"

#include <AccessibleWatcher.h>
#include "Commands/Commands.h"
#include "Commands/PostCommand.h"
#include "Commands/PreCommand.h"

#include "config.h"
#include "loguru.hpp"

using namespace grpc;
using namespace aurum;

aurumServiceImpl::aurumServiceImpl()
{
    AccessibleWatcher::getInstance();
}

::grpc::Status aurumServiceImpl::execute(Command* cmd)
{
    std::unique_ptr<PreCommand>  proxyPreCmd  = std::make_unique<PreCommand>(cmd);
    std::unique_ptr<PostCommand> proxyPostCmd = std::make_unique<PostCommand>(proxyPreCmd.get());
    ::grpc::Status rst = proxyPostCmd->execute();
    return rst;
}

aurumServiceImpl::~aurumServiceImpl() {}
::grpc::Status aurumServiceImpl::aurumServiceImpl::sync(
    ::grpc::ServerContext* context, const ::aurum::ReqEmpty* request,
    ::aurum::RspEmpty* response)
{
    std::unique_ptr<SyncCommand> cmd = std::make_unique<SyncCommand>(request, response);
    return execute(cmd.get());
}
::grpc::Status aurumServiceImpl::aurumServiceImpl::killServer(
    ::grpc::ServerContext* context, const ::aurum::ReqEmpty* request,
    ::aurum::RspEmpty* response)
{
    std::unique_ptr<KillServerCommand> cmd = std::make_unique<KillServerCommand>(request, response);
    return execute(cmd.get());
}
::grpc::Status aurumServiceImpl::aurumServiceImpl::findElement(
    ::grpc::ServerContext* context, const ::aurum::ReqFindElement* request,
    ::aurum::RspFindElement* response)
{
    std::unique_ptr<FindElementCommand> cmd = std::make_unique<FindElementCommand>(request, response);
    return execute(cmd.get());
}

::grpc::Status aurumServiceImpl::aurumServiceImpl::click(
    ::grpc::ServerContext* context, const ::aurum::ReqClick* request,
    ::aurum::RspClick* response)
{
    std::unique_ptr<ClickCommand> cmd = ClickCommand::createCommand(request, response);
    return execute(cmd.get());
}
::grpc::Status aurumServiceImpl::getValue(::grpc::ServerContext*      context,
                                          const ::aurum::ReqGetValue* request,
                                          ::aurum::RspGetValue*       response)
{
    std::unique_ptr<GetValueCommand> cmd = std::make_unique<GetValueCommand>(request, response);
    return execute(cmd.get());
}
::grpc::Status aurumServiceImpl::setValue(::grpc::ServerContext*      context,
                                          const ::aurum::ReqSetValue* request,
                                          ::aurum::RspSetValue*       response)
{
    std::unique_ptr<SetValueCommand> cmd = std::make_unique<SetValueCommand>(request, response);
    return execute(cmd.get());
}
::grpc::Status aurumServiceImpl::getAttribute(
    ::grpc::ServerContext* context, const ::aurum::ReqGetAttribute* request,
    ::aurum::RspGetAttribute* response)
{
    std::unique_ptr<GetAttributeCommand> cmd = GetAttributeCommand::createCommand(request, response);
    return execute(cmd.get());
}
::grpc::Status aurumServiceImpl::getSize(::grpc::ServerContext*     context,
                                         const ::aurum::ReqGetSize* request,
                                         ::aurum::RspGetSize*       response)
{
    std::unique_ptr<GetSizeCommand> cmd = std::make_unique<GetSizeCommand>(request, response);
    return execute(cmd.get());
}
::grpc::Status aurumServiceImpl::clear(::grpc::ServerContext*   context,
                                       const ::aurum::ReqClear* request,
                                       ::aurum::RspClear*       response)
{
    std::unique_ptr<ClearCommand> cmd = std::make_unique<ClearCommand>(request, response);
    return execute(cmd.get());
}
::grpc::Status aurumServiceImpl::installApp(
    ::grpc::ServerContext*                         context,
    ::grpc::ServerReader< ::aurum::ReqInstallApp>* request,
    ::aurum::RspInstallApp*                        response)
{
    std::unique_ptr<InstallAppCommand> cmd = std::make_unique<InstallAppCommand>(request, response);
    return execute(cmd.get());
}
::grpc::Status aurumServiceImpl::removeApp(::grpc::ServerContext*       context,
                                           const ::aurum::ReqRemoveApp* request,
                                           ::aurum::RspRemoveApp* response)
{
    std::unique_ptr<RemoveAppCommand> cmd = std::make_unique<RemoveAppCommand>(request, response);
    return execute(cmd.get());
}
::grpc::Status aurumServiceImpl::getAppInfo(
    ::grpc::ServerContext* context, const ::aurum::ReqGetAppInfo* request,
    ::aurum::RspGetAppInfo* response)
{
    std::unique_ptr<GetAppInfoCommand> cmd = std::make_unique<GetAppInfoCommand>(request, response);
    return execute(cmd.get());
}
::grpc::Status aurumServiceImpl::launchApp(::grpc::ServerContext*       context,
                                           const ::aurum::ReqLaunchApp* request,
                                           ::aurum::RspLaunchApp* response)
{
    std::unique_ptr<LaunchAppCommand> cmd = std::make_unique<LaunchAppCommand>(request, response);
    return execute(cmd.get());
}
::grpc::Status aurumServiceImpl::closeApp(::grpc::ServerContext*      context,
                                          const ::aurum::ReqCloseApp* request,
                                          ::aurum::RspCloseApp*       response)
{
    std::unique_ptr<CloseAppCommand> cmd = std::make_unique<CloseAppCommand>(request, response);
    return execute(cmd.get());
}
::grpc::Status aurumServiceImpl::touchDown(::grpc::ServerContext*       context,
                                           const ::aurum::ReqTouchDown* request,
                                           ::aurum::RspTouchDown* response)
{
    std::unique_ptr<TouchDownCommand> cmd = std::make_unique<TouchDownCommand>(request, response);
    return execute(cmd.get());
}
::grpc::Status aurumServiceImpl::touchUp(::grpc::ServerContext*     context,
                                         const ::aurum::ReqTouchUp* request,
                                         ::aurum::RspTouchUp*       response)
{
    std::unique_ptr<TouchUpCommand> cmd = std::make_unique<TouchUpCommand>(request, response);
    return execute(cmd.get());
}
::grpc::Status aurumServiceImpl::touchMove(::grpc::ServerContext*       context,
                                           const ::aurum::ReqTouchMove* request,
                                           ::aurum::RspTouchMove* response)
{
    std::unique_ptr<TouchMoveCommand> cmd = std::make_unique<TouchMoveCommand>(request, response);
    return execute(cmd.get());
}
::grpc::Status aurumServiceImpl::longClick(::grpc::ServerContext*   context,
                                           const ::aurum::ReqClick* request,
                                           ::aurum::RspClick*       response)
{
    std::unique_ptr<LongClickCommand> cmd = std::make_unique<LongClickCommand>(request, response);
    return execute(cmd.get());
}
::grpc::Status aurumServiceImpl::flick(::grpc::ServerContext*   context,
                                       const ::aurum::ReqFlick* request,
                                       ::aurum::RspFlick*       response)
{
    std::unique_ptr<FlickCommand> cmd = std::make_unique<FlickCommand>(request, response);
    return execute(cmd.get());
}
::grpc::Status aurumServiceImpl::getDeviceTime(
    ::grpc::ServerContext* context, const ::aurum::ReqGetDeviceTime* request,
    ::aurum::RspGetDeviceTime* response)
{
    std::unique_ptr<GetDeviceTimeCommand> cmd = std::make_unique<GetDeviceTimeCommand>(request, response);
    return execute(cmd.get());
}
::grpc::Status aurumServiceImpl::getLocation(
    ::grpc::ServerContext* context, const ::aurum::ReqGetLocation* request,
    ::aurum::RspGetLocation* response)
{
    std::unique_ptr<GetLocationCommand> cmd = std::make_unique<GetLocationCommand>(request, response);
    return execute(cmd.get());
}
::grpc::Status aurumServiceImpl::sendKey(::grpc::ServerContext* context,
                                         const ::aurum::ReqKey* request,
                                         ::aurum::RspKey*       response)
{
    std::unique_ptr<SendKeyCommand> cmd = std::make_unique<SendKeyCommand>(request, response);
    return execute(cmd.get());
}
