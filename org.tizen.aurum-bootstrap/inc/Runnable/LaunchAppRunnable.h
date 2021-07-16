#pragma once

#include <string>

#include "Runnable.h"

class LaunchAppRunnable : public Runnable {
protected:
	std::string mPkg;

public:
    LaunchAppRunnable(std::string pkg);
    std::string getPkgName();
    void run() const override;
};
