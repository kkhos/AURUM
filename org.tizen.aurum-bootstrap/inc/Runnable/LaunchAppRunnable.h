#pragma once

#include <string>

#include "Runnable.h"
#include <aurum.grpc.pb.h>

class LaunchAppRunnable : public Runnable {
protected:
    std::string mPkg;
    const google::protobuf::RepeatedPtrField<aurum::LaunchData>& mData;

public:
    LaunchAppRunnable(std::string pkg, const google::protobuf::RepeatedPtrField<aurum::LaunchData>& data);
    std::string getPkgName();
    void run() const override;
};
