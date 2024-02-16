#include <gtest/gtest.h>

#include <UiDevice.h>
#include <UiObject.h>
#include <UiSelector.h>
#include <Sel.h>

#include "MockDeviceImpl.h"

#include <iostream>
#include <algorithm>

#include "MockAccessibleWatcher.h"
#include "MockAccessibleAppManager.h"
#include "MockAccessibleApplication.h"
#include "MockAccessibleWindow.h"
#include "MockAccessibleNode.h"

using namespace Aurum;
using namespace AurumInternal::Mock;

class AurumTestSel : public ::testing::Test {
    public:
        AurumTestSel() : mDevice{nullptr}, mAppManager{nullptr}, mWatcher{nullptr}, mApps{}, mWins{}, mNodes{}{
        }
    protected:
        void SetUp() override {
            mDevice = new MockDeviceImpl();
            UiDevice::getInstance(mDevice);

            mWatcher = new MockAccessibleWatcher();
            AccessibleWatcher::getInstance(mWatcher);

            mAppManager = new MockAccessibleAppManager();
            AccessibleAppManager::getInstance(mAppManager);

            mApps.push_back(mAppManager->addApplication("org.tizen.aurum.test.app1", {0,0,1024,1024}, 0, 0));
            mApps.push_back(mAppManager->addApplication("org.tizen.aurum.test.app2", {0,0,1024,1024}, 0, 0));

            mWins.push_back(mApps[0]->addWindow("win1", "Elm_Win", {100,100,200,200}, (int)NodeFeatureProperties::SHOWING|(int)NodeFeatureProperties::VISIBLE|(int)NodeFeatureProperties::ACTIVE));
            mWins.push_back(mApps[0]->addWindow("win2", "Elm_Win", {200,200,200,200}, (int)NodeFeatureProperties::VISIBLE|(int)NodeFeatureProperties::SHOWING|(int)NodeFeatureProperties::ACTIVE));
            mWins.push_back(mApps[0]->addWindow("win3", "Elm_Win", {300,300,200,200}, (int)NodeFeatureProperties::VISIBLE));

            mNodes.push_back(mWins[0]->addNode("node1", "pkg", "TeSt1234!@#$", "res", "type", "style", "", {0,0,100,100}, 0, (int)NodeFeatureProperties::SHOWING|(int)NodeFeatureProperties::ENABLED));
            mNodes.push_back(mWins[0]->addNode("node2", "pkg", "TeSt1234!@#$", "res", "type", "style", "", {100,100,200,200}, 0, (int)NodeFeatureProperties::SHOWING));
            mNodes.push_back(mWins[0]->addNode("node3", "pkg", "TeSt1234!@#$", "res", "type", "style", "", {200,200,300,300}, 0, (int)NodeFeatureProperties::SHOWING|(int)NodeFeatureProperties::ACTIVE));
            mNodes.push_back(mWins[0]->addNode("node4", "pkg", "TeSt1234!@#$", "res", "type", "style", "", {200,200,300,300}, 0, 0));
            mNodes.push_back(mNodes[3]->addChild("node5", "pkg", "TeSt1234!@#$", "res", "type of widget", "style of widget", "", {200,200,300,300}, 0, 0));
        }

        void TearDown() override {
        }

        MockDeviceImpl *mDevice;
        MockAccessibleAppManager *mAppManager;
        MockAccessibleWatcher *mWatcher;
        std::vector<std::shared_ptr<MockAccessibleApplication>> mApps;
        std::vector<std::shared_ptr<MockAccessibleWindow>> mWins;
        std::vector<std::shared_ptr<MockAccessibleNode>> mNodes;
};

TEST_F(AurumTestSel, EmptyTest)
{
    ASSERT_EQ(true, true);
}

TEST_F(AurumTestSel, text_P1)
{
    auto founds = UiDevice::getInstance()->findObjects(Sel::text("node5"));
    ASSERT_EQ(founds.size(), 1);
    ASSERT_EQ(founds[0]->getText(), "node5");
}

TEST_F(AurumTestSel, type_P1)
{
    auto founds = UiDevice::getInstance()->findObjects(Sel::type("type of widget"));
    ASSERT_EQ(founds.size(), 1);
    ASSERT_EQ(founds[0]->getText(), "node5");
}

TEST_F(AurumTestSel, style_P1)
{
    auto founds = UiDevice::getInstance()->findObjects(Sel::style("style of widget"));
    ASSERT_EQ(founds.size(), 1);
    ASSERT_EQ(founds[0]->getText(), "node5");
}

TEST_F(AurumTestSel, depth_P1)
{
    int i = 0 ;
    int sum = 0;
    while(true){
       auto founds = UiDevice::getInstance()->findObjects(Sel::depth(i++));
       if (founds.size() <= 0) break;
       sum += founds.size();
    }
    ASSERT_EQ(sum, 16);
}

TEST_F(AurumTestSel, text_N1)
{
    auto founds = UiDevice::getInstance()->findObjects(Sel::text("node5"));
    ASSERT_NE(founds.size(), 2);
}

TEST_F(AurumTestSel, type_N1)
{
    auto founds = UiDevice::getInstance()->findObjects(Sel::type("type of widget"));
    ASSERT_NE(founds.size(), 2);
}

TEST_F(AurumTestSel, style_N1)
{
    auto founds = UiDevice::getInstance()->findObjects(Sel::style("style of widget"));
    ASSERT_NE(founds.size(), 0);
}

TEST_F(AurumTestSel, depth_N1)
{
    auto founds = UiDevice::getInstance()->findObjects(Sel::depth(1));
    ASSERT_NE(founds.size(), 10);
}
