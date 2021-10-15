/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd All Rights Reserved
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
