#pragma once

#include <string>

#include "Runnable.h"

class LaunchAppRunnable : public Runnable {
protected:
    std::string mPkg;
    std::string mKey;
    std::string mValue;

public:
    LaunchAppRunnable(std::string pkg, std::string key, std::string value);
    std::string getPkgName();
    void run() const override;
};
