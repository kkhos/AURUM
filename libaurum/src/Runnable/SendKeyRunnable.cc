#include "SendKeyRunnable.h"

#include <UiDevice.h>
#include <loguru.hpp>

SendKeyRunnable::SendKeyRunnable(std::string keycode)
    : mKeycode{keycode}
{
}

void SendKeyRunnable::run() const
{
	LOG_F(INFO, "sendKeyRUn %s",mKeycode.c_str());
	std::shared_ptr<UiDevice> mDevice = UiDevice::getInstance();
	mDevice->pressKeyCode(mKeycode.c_str(), KeyRequestType::PRESS);
	mDevice->pressKeyCode(mKeycode.c_str(), KeyRequestType::RELEASE);
}

