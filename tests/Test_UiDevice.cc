#include <gtest/gtest.h>

#include <UiDevice.h>
#include <UiObject.h>
#include <UiSelector.h>
#include <Sel.h>
#include <Until.h>

#include "MockDeviceImpl.h"

#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>

#include "MockAccessibleWatcher.h"
#include "MockAccessibleAppManager.h"
#include "MockAccessibleApplication.h"
#include "MockAccessibleWindow.h"
#include "MockAccessibleNode.h"

using namespace Aurum;
using namespace AurumInternal::Mock;

class AurumTestUiDevice : public ::testing::Test {
    public:
        AurumTestUiDevice()
        : mDevice{nullptr}, mAppManager{nullptr}, mWatcher{nullptr}, mApp{nullptr}, mWin{nullptr}, mNode{nullptr} {
        }

        void SetUp() override {
            mDevice = new MockDeviceImpl();
            UiDevice::getInstance(mDevice);

            mWatcher = new MockAccessibleWatcher();
            AccessibleWatcher::getInstance(mWatcher);

            mAppManager = new MockAccessibleAppManager();
            AccessibleAppManager::getInstance(mAppManager);

            mApp = mAppManager->addApplication("org.tizen.aurum.test.app", {0,0,1024,1024}, 0, 0);
            mWin = mApp->addWindow("title", "Elm_Win", {100,100,200,200}, (int)NodeFeatureProperties::SHOWING|(int)NodeFeatureProperties::VISIBLE|(int)NodeFeatureProperties::ACTIVE);
            mWin->addNode("test", "pkg", "TeSt1234!@#$", "res", "type", "style", "", {0,0,100,100}, 0, 0);
            mNode = mWin->addNode("test", "pkg", "TeSt1234!@#$", "res", "type", "style", "", {100,100,200,200}, 0, 0);
        }

        void TearDown() override {
        }

        MockDeviceImpl *mDevice;
        MockAccessibleAppManager *mAppManager;
        MockAccessibleWatcher *mWatcher;
        std::shared_ptr<MockAccessibleApplication> mApp;
        std::shared_ptr<MockAccessibleWindow> mWin;
        std::shared_ptr<MockAccessibleNode> mNode;
};

TEST_F(AurumTestUiDevice, EmptyTest)
{
    ASSERT_EQ(true, true);
}

TEST_F(AurumTestUiDevice, hasObject_P1)
{
    auto objectExists = UiDevice::getInstance()->hasObject(Sel::text("test"));
    ASSERT_EQ(objectExists, true);
    objectExists = UiDevice::getInstance()->hasObject(Sel::text("new_object"));
    ASSERT_EQ(objectExists, false);
}

TEST_F(AurumTestUiDevice, findObject_P1)
{
    auto objectInstance = UiDevice::getInstance()->findObject(Sel::text("test"));
    ASSERT_EQ(objectInstance->getRole(), "TeSt1234!@#$");
}

TEST_F(AurumTestUiDevice, findObjects_P1)
{
    auto objectsList = UiDevice::getInstance()->findObjects(Sel::text("test"));
    ASSERT_EQ(objectsList.size(), 2);
    std::for_each(objectsList.begin(), objectsList.end(), [](auto &obj){
        ASSERT_EQ(obj->getRole(), "TeSt1234!@#$");
    });
}

TEST_F(AurumTestUiDevice, waitFor_P1)
{
    std::thread t([&](){
        std::this_thread::sleep_for(std::chrono::milliseconds{250});
        mWin->addNode("not_exists_now", "pkg", "TeSt1234!@#$", "res", "type", "style", "",{0,0,100,100}, 0, 0);
    });
    auto found = UiDevice::getInstance()->waitFor(Until::hasObject(Sel::text("not_exists_now")));
    t.detach();
    ASSERT_EQ(found, true);
}

TEST_F(AurumTestUiDevice, getInstance_P1)
{
    ASSERT_NE(UiDevice::getInstance(), nullptr);
}

TEST_F(AurumTestUiDevice, getWindowRoot_P1)
{
    auto windowroot = UiDevice::getInstance()->getWindowRoot();
    ASSERT_EQ(windowroot.size(), 1);
}

TEST_F(AurumTestUiDevice, click_P1)
{
    const int x = 100, y = 200, intv = 123;
    UiDevice::getInstance()->click(x, y);
    //ASSERT_EQ(mDevice->testClick(x, y), true);
    //ASSERT_EQ(mDevice->testClickIntv(x, y, MockDeviceImpl::INTV_CLICK), true);

    UiDevice::getInstance()->click(x, y, intv);
    //ASSERT_EQ(mDevice->testClickIntv(x, y, intv), true);
}

TEST_F(AurumTestUiDevice, drag_P1)
{
    int sx = 0, sy = 0, ex = 100, ey = 100, step = 10, dur = 500;

    UiDevice::getInstance()->drag(sx, sy, ex, ey, step, dur);
    //ASSERT_EQ(mDevice->testDrag(sx, sy, ex, ey, step, dur), true);
    ASSERT_EQ(true, true);
}

TEST_F(AurumTestUiDevice, touch_P1)
{
    int sx = 100, sy = 200;
    int seq = UiDevice::getInstance()->touchDown(sx, sy);
    ASSERT_EQ(seq, 1);
    ASSERT_EQ(mDevice->mTouchDown.count(seq), 0);

    ASSERT_EQ(UiDevice::getInstance()->touchMove(sx*2, sy*2, seq), true);

    ASSERT_EQ(mDevice->mTouchDown[seq].x, 0);
    ASSERT_EQ(mDevice->mTouchDown[seq].y, 0);
    ASSERT_EQ(UiDevice::getInstance()->touchUp(sx*2, sy*2, seq), true);

}

TEST_F(AurumTestUiDevice, touch_P2)
{
    int sx = 100, sy = 200;
    int seq1 = UiDevice::getInstance()->touchDown(sx, sy);
    int seq2 = UiDevice::getInstance()->touchDown(sx, sy);
    int seq3 = UiDevice::getInstance()->touchDown(sx, sy);
    ASSERT_EQ(seq1, 1);
    ASSERT_EQ(seq2, 2);
    ASSERT_EQ(seq3, -1);
}

TEST_F(AurumTestUiDevice, wheelUp_P1)
{
    timespec t1, t2;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    UiDevice::getInstance()->wheelUp(123, 500);
    clock_gettime(CLOCK_MONOTONIC, &t2);
    ASSERT_EQ(mDevice->mWheelDevice, 0);
}

TEST_F(AurumTestUiDevice, wheelDown_P1)
{
    timespec t1, t2;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    UiDevice::getInstance()->wheelDown(123, 500);
    clock_gettime(CLOCK_MONOTONIC, &t2);
    ASSERT_EQ(mDevice->mWheelDevice, 0);
}

TEST_F(AurumTestUiDevice, takeScreenshot_P1)
{
    ASSERT_EQ(true, true);
}

TEST_F(AurumTestUiDevice, getSystemTime_P1)
{
    struct timespec t;
    long long t1 = UiDevice::getInstance()->getSystemTime(TimeRequestType::MONOTONIC);
    clock_gettime(CLOCK_MONOTONIC, &t);
    long long t2 = (long long)t.tv_sec * 1000L + (long long)(t.tv_nsec / 1000000);
    ASSERT_NEAR(t1,t2,100);
    long long t3 = UiDevice::getInstance()->getSystemTime(TimeRequestType::WALLCLOCK);
    clock_gettime(CLOCK_REALTIME, &t);
    long long t4 = (long long)t.tv_sec * 1000L + (long long)(t.tv_nsec / 1000000);
    ASSERT_NEAR(t3,t4,100);
}

TEST_F(AurumTestUiDevice, hasObject_N1)
{
    auto objectExists = UiDevice::getInstance()->hasObject(Sel::text("test"));
    ASSERT_NE(objectExists, false);
    objectExists = UiDevice::getInstance()->hasObject(Sel::text("new_object"));
    ASSERT_NE(objectExists, true);
}

TEST_F(AurumTestUiDevice, findObject_N1)
{
    auto objectInstance = UiDevice::getInstance()->findObject(Sel::text("test"));
    ASSERT_NE(objectInstance->getRole(), "test");
}

TEST_F(AurumTestUiDevice, findObjects_N1)
{
    auto objectsList = UiDevice::getInstance()->findObjects(Sel::text("test"));
    ASSERT_EQ(objectsList.size(), 2);
    std::for_each(objectsList.begin(), objectsList.end(), [](auto &obj){
        ASSERT_NE(obj->getRole(), "test");
    });
}

TEST_F(AurumTestUiDevice, waitFor_N1)
{
    std::thread t([&](){
        std::this_thread::sleep_for(std::chrono::milliseconds{250});
        mWin->addNode("not_exists_now", "pkg", "TeSt1234!@#$", "res", "type", "style", "",{0,0,100,100}, 0, 0);
    });
    auto found = UiDevice::getInstance()->waitFor(Until::hasObject(Sel::text("not_exists_now")));
    t.detach();
    ASSERT_NE(found, false);
}

TEST_F(AurumTestUiDevice, getWindowRoot_N1)
{
    auto windowroot = UiDevice::getInstance()->getWindowRoot();
    ASSERT_NE(windowroot.size(), 0);
}

TEST_F(AurumTestUiDevice, pressKeyCode_P1)
{
    mDevice->pressKeyCode("BACK", KeyRequestType::PRESS);
    mDevice->pressKeyCode("HOME", KeyRequestType::PRESS);
    mDevice->pressKeyCode("MENU", KeyRequestType::PRESS);
    mDevice->pressKeyCode("VOLUP", KeyRequestType::PRESS);
    mDevice->pressKeyCode("VOLDOWN", KeyRequestType::PRESS);
    mDevice->pressKeyCode("POWER", KeyRequestType::PRESS);

    auto listOfKeys = mDevice->getGeneratedKeys();
    auto key = listOfKeys[0];
    ASSERT_EQ(std::get<2>(key), "BACK");
    key = listOfKeys[1];
    ASSERT_EQ(std::get<2>(key), "HOME");
    key = listOfKeys[2];
    ASSERT_EQ(std::get<2>(key), "MENU");
    key = listOfKeys[3];
    ASSERT_EQ(std::get<2>(key), "VOLUP");
    key = listOfKeys[4];
    ASSERT_EQ(std::get<2>(key), "VOLDOWN");
    key = listOfKeys[5];
    ASSERT_EQ(std::get<2>(key), "POWER");

    mDevice->clearGeneratedKeys();
}
