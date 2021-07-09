#include "bootstrap.h"
#include "GetSizeCommand.h"
#include "UiObject.h"

GetSizeCommand::GetSizeCommand(const ::aurum::ReqGetSize *request,
                               ::aurum::RspGetSize *response)
    : mRequest{request}, mResponse{response}
{
}

::grpc::Status GetSizeCommand::execute()
{
    LOGI("GetSize --------------- ");
    ObjectMapper *mObjMap = ObjectMapper::getInstance();
    std::shared_ptr<UiObject> obj = mObjMap->getElement(mRequest->elementid());
    if (obj) {
        const Rect<int> &size = obj->getBoundingBox();
        ::aurum::Rect *rect = mResponse->mutable_size();
        rect->set_x(size.mTopLeft.x);
        rect->set_y(size.mTopLeft.y);
        rect->set_width(size.width());
        rect->set_height(size.height());
    }

    return grpc::Status::OK;
}