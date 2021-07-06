#include "Aurum.h"

SendKeyRunnable::SendKeyRunnable(std::string keycode)
    : mKeycode{keycode}
{
}

void SendKeyRunnable::run() const
{
	LOGI("sendKey %s",mKeycode.c_str());
	std::shared_ptr<UiDevice> mDevice = UiDevice::getInstance();
	mDevice->pressKeyCode(mKeycode.c_str(), KeyRequestType::PRESS);
	mDevice->pressKeyCode(mKeycode.c_str(), KeyRequestType::RELEASE);
}

