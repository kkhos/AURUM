#include "GetSizeCommand.h"
#include <loguru.hpp>

#include <UiObject.h>

GetSizeCommand::GetSizeCommand(const ::aurum::ReqGetSize* request,
                               ::aurum::RspGetSize*       response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status GetSizeCommand::execute()
{
    LOG_SCOPE_F(INFO, "GetSize --------------- ");
    ObjectMapper *mObjMap = ObjectMapper::getInstance();
    UiObject* obj = mObjMap->getElement(mRequest->elementid());

    const Rect<int> &size = obj->getBoundingBox();

    ::aurum::Rect rect;
    rect.set_x(size.mTopLeft.x);
    rect.set_y(size.mTopLeft.y);
    rect.set_width(size.width());
    rect.set_height(size.height());

    mResponse->mutable_size()->CopyFrom(rect);

    return grpc::Status::OK;
}