#include "MockDeviceImpl.h"
#include <loguru.hpp>

#include <functional>
#include <tuple>
#include <iostream>
#include <memory>

#include <stdlib.h>
#include <time.h>

#include <stdio.h>

MockDeviceImpl::MockDeviceImpl()
:tStart{}, isTimerStarted{false}, mTouchSeq{}
{
    printf("mockdevice ctor\n");printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
}

MockDeviceImpl::~MockDeviceImpl()
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
}

bool MockDeviceImpl::click(const int x, const int y)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return false;
}

bool MockDeviceImpl::click(const int x, const int y, const unsigned int intv)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return false;
}

bool MockDeviceImpl::drag(const int sx, const int sy, const int ex, const int ey,
            const int steps, const int durationMs)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return false;
}

int MockDeviceImpl::touchDown(const int x, const int y)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return -1;
}

bool MockDeviceImpl::touchMove(const int x, const int y, const int seq)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return false;
}

bool MockDeviceImpl::touchUp(const int x, const int y, const int seq)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return false;

}

bool MockDeviceImpl::wheelUp(int amount, const int durationMs)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return false;

}

bool MockDeviceImpl::wheelDown(int amount, const int durationMs)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return false;

}

bool MockDeviceImpl::pressBack(KeyRequestType type)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return false;

}

bool MockDeviceImpl::pressHome(KeyRequestType type)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return false;

}

bool MockDeviceImpl::pressMenu(KeyRequestType type)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return false;

}

bool MockDeviceImpl::pressVolUp(KeyRequestType type)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return false;

}

bool MockDeviceImpl::pressVolDown(KeyRequestType type)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return false;

}

bool MockDeviceImpl::pressPower(KeyRequestType type)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return false;

}

bool MockDeviceImpl::pressKeyCode(std::string keycode, KeyRequestType type)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return false;

}

bool MockDeviceImpl::takeScreenshot(std::string path, float scale, int quality)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return false;

}

long long MockDeviceImpl::getSystemTime(TimeRequestType type)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return -1;
}

bool MockDeviceImpl::strokeKeyCode(std::string keycode, unsigned int intv)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return false;

}

bool MockDeviceImpl::pressKeyCode(std::string keycode)
{

    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return false;

}

bool MockDeviceImpl::releaseKeyCode(std::string keycode)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return false;

}

int MockDeviceImpl::grabTouchSeqNumber()
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return 0;
}

bool MockDeviceImpl::releaseTouchSeqNumber(int seq)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return false;

}

void MockDeviceImpl::startTimer(void)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
}

int MockDeviceImpl::stopTimer(void)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
    return 0;
}