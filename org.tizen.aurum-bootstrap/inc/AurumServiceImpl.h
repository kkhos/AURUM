#ifndef aurum_IMPL_H
#define aurum_IMPL_H

#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "aurum.grpc.pb.h"

class aurumServiceImpl final : public aurum::Bootstrap::Service {
public:
    aurumServiceImpl();
    virtual ~aurumServiceImpl();

protected:
    ::grpc::Status execute(Command *cmd);

public:
    ::grpc::Status sync(::grpc::ServerContext *  context,
                        const ::aurum::ReqEmpty *request,
                        ::aurum::RspEmpty *      response) override;
    ::grpc::Status killServer(::grpc::ServerContext *  context,
                              const ::aurum::ReqEmpty *request,
                              ::aurum::RspEmpty *      response) override;

    ::grpc::Status findElement(::grpc::ServerContext *        context,
                               const ::aurum::ReqFindElement *request,
                               ::aurum::RspFindElement *response) override;

    ::grpc::Status getValue(::grpc::ServerContext *     context,
                            const ::aurum::ReqGetValue *request,
                            ::aurum::RspGetValue *      response) override;
    ::grpc::Status setValue(::grpc::ServerContext *     context,
                            const ::aurum::ReqSetValue *request,
                            ::aurum::RspSetValue *      response) override;
    ::grpc::Status getAttribute(::grpc::ServerContext *         context,
                                const ::aurum::ReqGetAttribute *request,
                                ::aurum::RspGetAttribute *response) override;
    ::grpc::Status getSize(::grpc::ServerContext *    context,
                           const ::aurum::ReqGetSize *request,
                           ::aurum::RspGetSize *      response) override;
    ::grpc::Status clear(::grpc::ServerContext *  context,
                         const ::aurum::ReqClear *request,
                         ::aurum::RspClear *      response) override;
    ::grpc::Status installApp(
        ::grpc::ServerContext *                        context,
        ::grpc::ServerReader< ::aurum::ReqInstallApp> *reader,
        ::aurum::RspInstallApp *                       response) override;
    ::grpc::Status removeApp(::grpc::ServerContext *      context,
                             const ::aurum::ReqRemoveApp *request,
                             ::aurum::RspRemoveApp *      response) override;
    ::grpc::Status getAppInfo(::grpc::ServerContext *       context,
                              const ::aurum::ReqGetAppInfo *request,
                              ::aurum::RspGetAppInfo *      response) override;
    ::grpc::Status launchApp(::grpc::ServerContext *      context,
                             const ::aurum::ReqLaunchApp *request,
                             ::aurum::RspLaunchApp *      response) override;
    ::grpc::Status closeApp(::grpc::ServerContext *     context,
                            const ::aurum::ReqCloseApp *request,
                            ::aurum::RspCloseApp *      response) override;

    ::grpc::Status click(::grpc::ServerContext *  context,
                         const ::aurum::ReqClick *request,
                         ::aurum::RspClick *      response) override;
    ::grpc::Status longClick(::grpc::ServerContext *  context,
                             const ::aurum::ReqClick *request,
                             ::aurum::RspClick *      response) override;
    ::grpc::Status flick(::grpc::ServerContext *  context,
                         const ::aurum::ReqFlick *request,
                         ::aurum::RspFlick *      response) override;

    ::grpc::Status touchDown(::grpc::ServerContext *      context,
                             const ::aurum::ReqTouchDown *request,
                             ::aurum::RspTouchDown *      response) override;
    ::grpc::Status touchUp(::grpc::ServerContext *    context,
                           const ::aurum::ReqTouchUp *request,
                           ::aurum::RspTouchUp *      response) override;
    ::grpc::Status touchMove(::grpc::ServerContext *      context,
                             const ::aurum::ReqTouchMove *request,
                             ::aurum::RspTouchMove *      response) override;

    ::grpc::Status getDeviceTime(::grpc::ServerContext *          context,
                                 const ::aurum::ReqGetDeviceTime *request,
                                 ::aurum::RspGetDeviceTime *response) override;
    ::grpc::Status getLocation(::grpc::ServerContext *        context,
                               const ::aurum::ReqGetLocation *request,
                               ::aurum::RspGetLocation *response) override;
    ::grpc::Status sendKey(::grpc::ServerContext *context,
                           const ::aurum::ReqKey *request,
                           ::aurum::RspKey *      response) override;
};

#endif