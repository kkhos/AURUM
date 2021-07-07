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
#include "MockAccessibleApplication.h"
#include "MockAccessibleWindow.h"
#include "MockAccessibleNode.h"


class AurumTestUiDevice : public ::testing::Test {
    public:
        AurumTestUiDevice()
        : mDevice{nullptr}, mWatcher{nullptr}, mApp{nullptr}, mWin{nullptr}, mNode{nullptr} {
        }

        void SetUp() override {
            mDevice = new MockDeviceImpl();
            UiDevice::getInstance(mDevice);

            mWatcher = new MockAccessibleWatcher();
            AccessibleWatcher::getInstance(mWatcher);

            mApp = mWatcher->addApplication("org.tizen.aurum.test.app", {0,0,1024,1024}, 0, 0);
            mWin = mApp->addWindow("title", "Elm_Win", {100,100,200,200}, (int)NodeFeatureProperties::SHOWING|(int)NodeFeatureProperties::VISIBLE|(int)NodeFeatureProperties::ACTIVE);
            mWin->addNode("test", "pkg", "TeSt1234!@#$", "res", "type", "style", "", {0,0,100,100}, 0, 0);
            mNode = mWin->addNode("test", "pkg", "TeSt1234!@#$", "res", "type", "style", "", {100,100,200,200}, 0, 0);
        }

        void TearDown() override {
        }

        MockDeviceImpl *mDevice;
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
    ASSERT_EQ(mDevice->mTouchDown.count(seq), 1);

    ASSERT_EQ(UiDevice::getInstance()->touchMove(sx*2, sy*2, seq), true);

    ASSERT_EQ(mDevice->mTouchDown[seq].x, sx*2);
    ASSERT_EQ(mDevice->mTouchDown[seq].y, sy*2);
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
    ASSERT_EQ(mDevice->mWheelDevice, 123);
}

TEST_F(AurumTestUiDevice, wheelDown_P1)
{
    timespec t1, t2;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    UiDevice::getInstance()->wheelDown(123, 500);
    clock_gettime(CLOCK_MONOTONIC, &t2);
    ASSERT_EQ(mDevice->mWheelDevice, -123);
}

TEST_F(AurumTestUiDevice, pressBack_P1)
{
    UiDevice::getInstance()->pressBack(KeyRequestType::PRESS);
    UiDevice::getInstance()->pressBack(KeyRequestType::RELEASE);
    UiDevice::getInstance()->pressBack(KeyRequestType::STROKE);
    UiDevice::getInstance()->pressBack(KeyRequestType::LONG_STROKE);

    ASSERT_EQ(mDevice->mKeyDevice.size(), 4);

    ASSERT_EQ(std::get<0>(mDevice->mKeyDevice[0]), MockKeyType::BACK);
    ASSERT_EQ(std::get<1>(mDevice->mKeyDevice[0]), KeyRequestType::PRESS);

    ASSERT_EQ(std::get<0>(mDevice->mKeyDevice[1]), MockKeyType::BACK);
    ASSERT_EQ(std::get<1>(mDevice->mKeyDevice[1]), KeyRequestType::RELEASE);

    ASSERT_EQ(std::get<0>(mDevice->mKeyDevice[2]), MockKeyType::BACK);
    ASSERT_EQ(std::get<1>(mDevice->mKeyDevice[2]), KeyRequestType::STROKE);

    ASSERT_EQ(std::get<0>(mDevice->mKeyDevice[3]), MockKeyType::BACK);
    ASSERT_EQ(std::get<1>(mDevice->mKeyDevice[3]), KeyRequestType::LONG_STROKE);
}

TEST_F(AurumTestUiDevice, pressHome_P1)
{
    UiDevice::getInstance()->pressHome(KeyRequestType::PRESS);
    UiDevice::getInstance()->pressHome(KeyRequestType::RELEASE);
    UiDevice::getInstance()->pressHome(KeyRequestType::STROKE);
    UiDevice::getInstance()->pressHome(KeyRequestType::LONG_STROKE);

    ASSERT_EQ(mDevice->mKeyDevice.size(), 4);

    ASSERT_EQ(std::get<0>(mDevice->mKeyDevice[0]), MockKeyType::HOME);
    ASSERT_EQ(std::get<1>(mDevice->mKeyDevice[0]), KeyRequestType::PRESS);

    ASSERT_EQ(std::get<0>(mDevice->mKeyDevice[1]), MockKeyType::HOME);
    ASSERT_EQ(std::get<1>(mDevice->mKeyDevice[1]), KeyRequestType::RELEASE);

    ASSERT_EQ(std::get<0>(mDevice->mKeyDevice[2]), MockKeyType::HOME);
    ASSERT_EQ(std::get<1>(mDevice->mKeyDevice[2]), KeyRequestType::STROKE);

    ASSERT_EQ(std::get<0>(mDevice->mKeyDevice[3]), MockKeyType::HOME);
    ASSERT_EQ(std::get<1>(mDevice->mKeyDevice[3]), KeyRequestType::LONG_STROKE);
}

TEST_F(AurumTestUiDevice, pressVolUp_P1)
{
    UiDevice::getInstance()->pressVolUp(KeyRequestType::PRESS);
    UiDevice::getInstance()->pressVolUp(KeyRequestType::RELEASE);
    UiDevice::getInstance()->pressVolUp(KeyRequestType::STROKE);
    UiDevice::getInstance()->pressVolUp(KeyRequestType::LONG_STROKE);

    ASSERT_EQ(mDevice->mKeyDevice.size(), 4);

    ASSERT_EQ(std::get<0>(mDevice->mKeyDevice[0]), MockKeyType::VOLUP);
    ASSERT_EQ(std::get<1>(mDevice->mKeyDevice[0]), KeyRequestType::PRESS);

    ASSERT_EQ(std::get<0>(mDevice->mKeyDevice[1]), MockKeyType::VOLUP);
    ASSERT_EQ(std::get<1>(mDevice->mKeyDevice[1]), KeyRequestType::RELEASE);

    ASSERT_EQ(std::get<0>(mDevice->mKeyDevice[2]), MockKeyType::VOLUP);
    ASSERT_EQ(std::get<1>(mDevice->mKeyDevice[2]), KeyRequestType::STROKE);

    ASSERT_EQ(std::get<0>(mDevice->mKeyDevice[3]), MockKeyType::VOLUP);
    ASSERT_EQ(std::get<1>(mDevice->mKeyDevice[3]), KeyRequestType::LONG_STROKE);
}

TEST_F(AurumTestUiDevice, pressVolDown_P1)
{
    UiDevice::getInstance()->pressVolDown(KeyRequestType::PRESS);
    UiDevice::getInstance()->pressVolDown(KeyRequestType::RELEASE);
    UiDevice::getInstance()->pressVolDown(KeyRequestType::STROKE);
    UiDevice::getInstance()->pressVolDown(KeyRequestType::LONG_STROKE);

    ASSERT_EQ(mDevice->mKeyDevice.size(), 4);

    ASSERT_EQ(std::get<0>(mDevice->mKeyDevice[0]), MockKeyType::VOLDOWN);
    ASSERT_EQ(std::get<1>(mDevice->mKeyDevice[0]), KeyRequestType::PRESS);

    ASSERT_EQ(std::get<0>(mDevice->mKeyDevice[1]), MockKeyType::VOLDOWN);
    ASSERT_EQ(std::get<1>(mDevice->mKeyDevice[1]), KeyRequestType::RELEASE);

    ASSERT_EQ(std::get<0>(mDevice->mKeyDevice[2]), MockKeyType::VOLDOWN);
    ASSERT_EQ(std::get<1>(mDevice->mKeyDevice[2]), KeyRequestType::STROKE);

    ASSERT_EQ(std::get<0>(mDevice->mKeyDevice[3]), MockKeyType::VOLDOWN);
    ASSERT_EQ(std::get<1>(mDevice->mKeyDevice[3]), KeyRequestType::LONG_STROKE);
}

TEST_F(AurumTestUiDevice, pressPower_P1)
{
    UiDevice::getInstance()->pressPower(KeyRequestType::PRESS);
    UiDevice::getInstance()->pressPower(KeyRequestType::RELEASE);
    UiDevice::getInstance()->pressPower(KeyRequestType::STROKE);
    UiDevice::getInstance()->pressPower(KeyRequestType::LONG_STROKE);

    ASSERT_EQ(mDevice->mKeyDevice.size(), 4);

    ASSERT_EQ(std::get<0>(mDevice->mKeyDevice[0]), MockKeyType::POWER);
    ASSERT_EQ(std::get<1>(mDevice->mKeyDevice[0]), KeyRequestType::PRESS);

    ASSERT_EQ(std::get<0>(mDevice->mKeyDevice[1]), MockKeyType::POWER);
    ASSERT_EQ(std::get<1>(mDevice->mKeyDevice[1]), KeyRequestType::RELEASE);

    ASSERT_EQ(std::get<0>(mDevice->mKeyDevice[2]), MockKeyType::POWER);
    ASSERT_EQ(std::get<1>(mDevice->mKeyDevice[2]), KeyRequestType::STROKE);

    ASSERT_EQ(std::get<0>(mDevice->mKeyDevice[3]), MockKeyType::POWER);
    ASSERT_EQ(std::get<1>(mDevice->mKeyDevice[3]), KeyRequestType::LONG_STROKE);
}

TEST_F(AurumTestUiDevice, pressKeyCode_P1)
{
    UiDevice::getInstance()->pressKeyCode("test::code::1", KeyRequestType::PRESS);
    UiDevice::getInstance()->pressKeyCode("test::code::2",KeyRequestType::RELEASE);
    UiDevice::getInstance()->pressKeyCode("test::code::3",KeyRequestType::STROKE);
    UiDevice::getInstance()->pressKeyCode("test::code::4",KeyRequestType::LONG_STROKE);

    ASSERT_EQ(mDevice->mKeyDevice.size(), 4);

    ASSERT_EQ(std::get<0>(mDevice->mKeyDevice[0]), MockKeyType::KEY);
    ASSERT_EQ(std::get<1>(mDevice->mKeyDevice[0]), KeyRequestType::PRESS);
    ASSERT_EQ(std::get<2>(mDevice->mKeyDevice[0]), "test::code::1");

    ASSERT_EQ(std::get<0>(mDevice->mKeyDevice[1]), MockKeyType::KEY);
    ASSERT_EQ(std::get<1>(mDevice->mKeyDevice[1]), KeyRequestType::RELEASE);
    ASSERT_EQ(std::get<2>(mDevice->mKeyDevice[1]), "test::code::2");

    ASSERT_EQ(std::get<0>(mDevice->mKeyDevice[2]), MockKeyType::KEY);
    ASSERT_EQ(std::get<1>(mDevice->mKeyDevice[2]), KeyRequestType::STROKE);
    ASSERT_EQ(std::get<2>(mDevice->mKeyDevice[2]), "test::code::3");

    ASSERT_EQ(std::get<0>(mDevice->mKeyDevice[3]), MockKeyType::KEY);
    ASSERT_EQ(std::get<1>(mDevice->mKeyDevice[3]), KeyRequestType::LONG_STROKE);
    ASSERT_EQ(std::get<2>(mDevice->mKeyDevice[3]), "test::code::4");
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
