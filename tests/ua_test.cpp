#include <gtest/gtest.h>

#include <UiDevice.h>
#include <UiObject.h>
#include <UiSelector.h>
#include <Sel.h>

#include "MockDeviceImpl.h"

#include <iostream>
#include <algorithm>

#include <loguru.hpp>


#include "MockAccessibleWatcher.h"
#include "MockAccessibleApplication.h"
#include "MockAccessibleWindow.h"
#include "MockAccessibleNode.h"


class UaTest : public ::testing::Test {
    public:
        UaTest(){
            /*const char *logPath = "/tmp/ua.log";
            loguru::g_preamble = false;
            loguru::add_file(logPath, loguru::Append, loguru::Verbosity_MAX);*/
        }
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }
};


TEST_F(UaTest, EmptyTest)
{
    ASSERT_EQ(true, true);
}



TEST_F(UaTest, DeviceInit)
{
    std::shared_ptr<UiDevice> mDevice1 = UiDevice::getInstance(nullptr);
    ASSERT_NE(mDevice1, nullptr);
}


TEST_F(UaTest, TextSelector)
{
    std::shared_ptr<UiSelector> sel = Sel::text("test");
    ASSERT_NE(sel.get(), nullptr);
    auto result = UiDevice::getInstance()->findObjects(sel);
    std::for_each(result.begin(), result.end(), [](auto obj){
        std::cout << obj->getText() << std::endl;
    });
    ASSERT_EQ(nullptr, nullptr);
}

TEST_F(UaTest, DeviceClick)
{
    auto result = UiDevice::getInstance()->click(1,2);
    ASSERT_EQ(result, false);
}

TEST_F(UaTest, FindElement)
{
    auto watcher = AccessibleWatcher::getInstance();

    MockAccessibleWatcher *mockWatcher = dynamic_cast<MockAccessibleWatcher*>(const_cast<AccessibleWatcher*>(watcher));

    std::shared_ptr<MockAccessibleNode> appNode = std::make_shared<MockAccessibleNode>(nullptr, "text", "pkg", "application", "res","type","style", Rect<int>{0,0,100,200}, 0, 0);
    std::shared_ptr<MockAccessibleApplication> app = std::make_shared<MockAccessibleApplication>(appNode);
    mockWatcher->addApplication(app);

    std::shared_ptr<MockAccessibleNode> winNode = std::make_shared<MockAccessibleNode>(nullptr, "text", "pkg", "window", "res","type","style", Rect<int>{0,0,100,200}, 0, 0);
    std::shared_ptr<MockAccessibleWindow> win = std::make_shared<MockAccessibleWindow>(app, winNode);
    winNode->setFeatureProperty(8);
    winNode->setFeatureProperty(9);
    winNode->setFeatureProperty(10);
    app->addWindow(win);

    std::shared_ptr<MockAccessibleNode> node = std::make_shared<MockAccessibleNode>(nullptr, "test", "pkg", "ahahah", "res","type","style", Rect<int>{0,0,100,200}, 0, 0);
    winNode->addChild(node);

    std::shared_ptr<UiSelector> sel = Sel::text("test");
    auto result = UiDevice::getInstance()->findObjects(sel);
    std::for_each(result.begin(), result.end(), [](auto obj){
        ASSERT_EQ(obj->getRole(), "ahahah");
    });

    ASSERT_EQ(nullptr, nullptr);
}