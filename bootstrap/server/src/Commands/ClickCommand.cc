#include "ClickCommand.h"

#include "UiObject.h"
#include "UiDevice.h"

#include <loguru.hpp>

ClickCommand::ClickCommand(const ::aurum::ReqClick* request,
                           ::aurum::RspClick*       response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status ClickCommand::execute()
{
   return grpc::Status::CANCELLED;
}

std::unique_ptr<ClickCommand> ClickCommand::createCommand(const ::aurum::ReqClick* request, ::aurum::RspClick* response)
{
    ::aurum::ReqClick_RequestType type = request->type();

    if (type == ::aurum::ReqClick_RequestType::ReqClick_RequestType_ELEMENTID)
        return std::make_unique<ClickElementCommand>(request, response);
    else if (type == ::aurum::ReqClick_RequestType::ReqClick_RequestType_COORD)
        return std::make_unique<ClickCoordCommand>(request, response);
    else if (type == ::aurum::ReqClick_RequestType::ReqClick_RequestType_ATSPI)
        return std::make_unique<ClickAtspiCommand>(request, response);
    else
        return std::make_unique<ClickCommand>(request, response);
}

::grpc::Status ClickElementCommand::execute()
{
    ObjectMapper* mObjMap = ObjectMapper::getInstance();
    UiObject* obj = mObjMap->getElement(mRequest->elementid());

    if (obj) {
        obj->click();
        mResponse->set_status(::aurum::RspStatus::OK);
    } else
        mResponse->set_status(::aurum::RspStatus::ERROR);

    return grpc::Status::OK;
}

::grpc::Status ClickCoordCommand::execute()
{
    UiDevice* obj = UiDevice::getInstance(DeviceType::DEFAULT);
    const ::aurum::Point& point = mRequest->coordination();
    obj->click(point.x(), point.y());
    mResponse->set_status(::aurum::RspStatus::OK);
}

::grpc::Status ClickAtspiCommand::execute()
{
    return grpc::Status::CANCELLED;
}
