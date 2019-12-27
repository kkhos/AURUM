#include "GetAttributeCommand.h"
#include <loguru.hpp>

#include <UiDevice.h>
#include <UiObject.h>

GetAttributeCommand::GetAttributeCommand(
    const ::aurum::ReqGetAttribute* request, ::aurum::RspGetAttribute* response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status GetAttributeCommand::execute()
{
    LOG_SCOPE_F(INFO, "GetAttribute --------------- ");
    ObjectMapper* mObjMap = ObjectMapper::getInstance();
    UiObject*     obj = mObjMap->getElement(mRequest->elementid());

    ::aurum::ReqGetAttribute_RequestType type = mRequest->attribute();
    AttributeGetter *getter = AttributeGetter::Creator(type);

    if (getter)
        getter->getPerform(obj, mResponse);

    return grpc::Status::OK;
}

AttributeGetter* AttributeGetter::Creator(::aurum::ReqGetAttribute_RequestType type)
{
    switch(type)
    {
        case ::aurum::ReqGetAttribute_RequestType::ReqGetAttribute_RequestType_VISIBLE:
            return new VisibleGetter();
        default:
            return nullptr;
    }
}

bool VisibleGetter::getPerform(UiObject *obj, ::aurum::RspGetAttribute* rsp)
{
    bool isVisible = obj->isVisible();

    rsp->set_boolvalue(isVisible);
    rsp->set_status(aurum::RspStatus::OK);

    return true;
}
AttributeGetter::~AttributeGetter(){}
VisibleGetter::~VisibleGetter(){}