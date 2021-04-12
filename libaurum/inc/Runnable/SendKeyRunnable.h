#pragma once

#include <string>

#include "Runnable.h"

class SendKeyRunnable : public Runnable {
protected:
	std::string mKeycode;

public:
    SendKeyRunnable(std::string keycode);
    void run() const override;
};
