#ifndef GET_ATTRIBUTE_COMMAND_H
#define GET_ATTRIBUTE_COMMAND_H

#include <gio/gio.h>
#include <grpcpp/grpcpp.h>
#include "Commands/Command.h"
#include "ObjectMapper.h"
#include "aurum.grpc.pb.h"
#include "config.h"

class GetAttributeCommand : public Command {
private:
    const ::aurum::ReqGetAttribute* mRequest;
    ::aurum::RspGetAttribute*       mResponse;

public:
    GetAttributeCommand(const ::aurum::ReqGetAttribute* request,
                        ::aurum::RspGetAttribute*       response);
    ;
    ::grpc::Status execute() override;
};

class AttributeGetter {
private:
public:
    static AttributeGetter *Creator(::aurum::ReqGetAttribute_RequestType type);
    virtual bool getPerform(UiObject *obj,  ::aurum::RspGetAttribute* mResponse){}
};

class VisibleGetter : public AttributeGetter{
private:
public:
    bool getPerform(UiObject *obj, ::aurum::RspGetAttribute* rsp) override;
};

#endif