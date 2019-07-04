#include "AurumServiceImpl.h"

#include <Accessible.h>
#include "Commands/Commands.h"
#include "Commands/PostCommand.h"
#include "Commands/PreCommand.h"

#include "config.h"
#include "loguru.hpp"

using namespace grpc;
using namespace aurum;

aurumServiceImpl::aurumServiceImpl()
{
    Accessible::getInstance();
}

::grpc::Status aurumServiceImpl::execute(Command& cmd)
{
    PreCommand     proxyPreCmd{&cmd};
    PostCommand    proxyPostCmd{&proxyPreCmd};
    ::grpc::Status rst = proxyPostCmd.execute();
    return rst;
}

aurumServiceImpl::~aurumServiceImpl() {}
::grpc::Status aurumServiceImpl::aurumServiceImpl::sync(
    ::grpc::ServerContext* context, const ::aurum::ReqEmpty* request,
    ::aurum::RspEmpty* response)
{
    SyncCommand cmd(request, response);
    return execute(cmd);
}
::grpc::Status aurumServiceImpl::aurumServiceImpl::killServer(
    ::grpc::ServerContext* context, const ::aurum::ReqEmpty* request,
    ::aurum::RspEmpty* response)
{
    KillServerCommand cmd(request, response);
    return execute(cmd);
}
::grpc::Status aurumServiceImpl::aurumServiceImpl::findElement(
    ::grpc::ServerContext* context, const ::aurum::ReqFindElement* request,
    ::aurum::RspFindElement* response)
{
    FindElementCommand cmd(request, response);
    return execute(cmd);
}
::grpc::Status aurumServiceImpl::aurumServiceImpl::click(
    ::grpc::ServerContext* context, const ::aurum::ReqClick* request,
    ::aurum::RspClick* response)
{
    ClickCommand cmd(request, response);
    return execute(cmd);
}
::grpc::Status aurumServiceImpl::getValue(::grpc::ServerContext*      context,
                                          const ::aurum::ReqGetValue* request,
                                          ::aurum::RspGetValue*       response)
{
    GetValueCommand cmd(request, response);
    return execute(cmd);
}
::grpc::Status aurumServiceImpl::setValue(::grpc::ServerContext*      context,
                                          const ::aurum::ReqSetValue* request,
                                          ::aurum::RspSetValue*       response)
{
    SetValueCommand cmd(request, response);
    return execute(cmd);
}
::grpc::Status aurumServiceImpl::getAttribute(
    ::grpc::ServerContext* context, const ::aurum::ReqGetAttribute* request,
    ::aurum::RspGetAttribute* response)
{
    GetAttributeCommand cmd(request, response);
    return execute(cmd);
}
::grpc::Status aurumServiceImpl::getSize(::grpc::ServerContext*     context,
                                         const ::aurum::ReqGetSize* request,
                                         ::aurum::RspGetSize*       response)
{
    GetSizeCommand cmd(request, response);
    return execute(cmd);
}
::grpc::Status aurumServiceImpl::clear(::grpc::ServerContext*   context,
                                       const ::aurum::ReqClear* request,
                                       ::aurum::RspClear*       response)
{
    ClearCommand cmd(request, response);
    return execute(cmd);
}
::grpc::Status aurumServiceImpl::installApp(
    ::grpc::ServerContext*                         context,
    ::grpc::ServerReader< ::aurum::ReqInstallApp>* request,
    ::aurum::RspInstallApp*                        response)
{
    InstallAppCommand cmd(request, response);
    return execute(cmd);
}
::grpc::Status aurumServiceImpl::removeApp(::grpc::ServerContext*       context,
                                           const ::aurum::ReqRemoveApp* request,
                                           ::aurum::RspRemoveApp* response)
{
    RemoveAppCommand cmd(request, response);
    return execute(cmd);
}
::grpc::Status aurumServiceImpl::getAppInfo(
    ::grpc::ServerContext* context, const ::aurum::ReqGetAppInfo* request,
    ::aurum::RspGetAppInfo* response)
{
    GetAppInfoCommand cmd(request, response);
    return execute(cmd);
}
::grpc::Status aurumServiceImpl::launchApp(::grpc::ServerContext*       context,
                                           const ::aurum::ReqLaunchApp* request,
                                           ::aurum::RspLaunchApp* response)
{
    LaunchAppCommand cmd(request, response);
    return execute(cmd);
}
::grpc::Status aurumServiceImpl::closeApp(::grpc::ServerContext*      context,
                                          const ::aurum::ReqCloseApp* request,
                                          ::aurum::RspCloseApp*       response)
{
    CloseAppCommand cmd(request, response);
    return execute(cmd);
}
::grpc::Status aurumServiceImpl::touchDown(::grpc::ServerContext*       context,
                                           const ::aurum::ReqTouchDown* request,
                                           ::aurum::RspTouchDown* response)
{
    TouchDownCommand cmd(request, response);
    return execute(cmd);
}
::grpc::Status aurumServiceImpl::touchUp(::grpc::ServerContext*     context,
                                         const ::aurum::ReqTouchUp* request,
                                         ::aurum::RspTouchUp*       response)
{
    TouchUpCommand cmd(request, response);
    return execute(cmd);
}
::grpc::Status aurumServiceImpl::touchMove(::grpc::ServerContext*       context,
                                           const ::aurum::ReqTouchMove* request,
                                           ::aurum::RspTouchMove* response)
{
    TouchMoveCommand cmd(request, response);
    return execute(cmd);
}
::grpc::Status aurumServiceImpl::longClick(::grpc::ServerContext*   context,
                                           const ::aurum::ReqClick* request,
                                           ::aurum::RspClick*       response)
{
    LongClickCommand cmd(request, response);
    return execute(cmd);
}
::grpc::Status aurumServiceImpl::flick(::grpc::ServerContext*   context,
                                       const ::aurum::ReqFlick* request,
                                       ::aurum::RspFlick*       response)
{
    FlickCommand cmd(request, response);
    return execute(cmd);
}
::grpc::Status aurumServiceImpl::getDeviceTime(
    ::grpc::ServerContext* context, const ::aurum::ReqGetDeviceTime* request,
    ::aurum::RspGetDeviceTime* response)
{
    GetDeviceTimeCommand cmd(request, response);
    return execute(cmd);
}
::grpc::Status aurumServiceImpl::getLocation(
    ::grpc::ServerContext* context, const ::aurum::ReqGetLocation* request,
    ::aurum::RspGetLocation* response)
{
    GetLocationCommand cmd(request, response);
    return execute(cmd);
}
::grpc::Status aurumServiceImpl::sendKey(::grpc::ServerContext* context,
                                         const ::aurum::ReqKey* request,
                                         ::aurum::RspKey*       response)
{
    SendKeyCommand cmd(request, response);
    return execute(cmd);
}
