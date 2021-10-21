#include <gtest/gtest.h>

#include <UiDevice.h>
#include <UiObject.h>
#include <UiSelector.h>
#include <Sel.h>

#include "MockDeviceImpl.h"

#include <iostream>
#include <algorithm>



#include "MockAccessibleWatcher.h"
#include "MockAccessibleApplication.h"
#include "MockAccessibleWindow.h"
#include "MockAccessibleNode.h"


class AurumTestUiSelector : public ::testing::Test {
    public:
        AurumTestUiSelector(): mDevice{nullptr}, mWatcher{nullptr}, mApps{}, mWins{}, mNodes{}{
        }
    protected:
        void SetUp() override {
            mDevice = new MockDeviceImpl();
            UiDevice::getInstance(mDevice);

            mWatcher = new MockAccessibleWatcher();
            AccessibleWatcher::getInstance(mWatcher);

            mApps.push_back(mWatcher->addApplication("org.tizen.aurum.test.app1", {0,0,1024,1024}, 0, 0));
            mApps.push_back(mWatcher->addApplication("org.tizen.aurum.test.app2", {0,0,1024,1024}, 0, 0));

            mWins.push_back(mApps[0]->addWindow("win1", "Elm_Win", {100,100,200,200}, (int)NodeFeatureProperties::SHOWING|(int)NodeFeatureProperties::VISIBLE|(int)NodeFeatureProperties::ACTIVE));
            mWins.push_back(mApps[0]->addWindow("win2", "Elm_Win", {200,200,200,200}, (int)NodeFeatureProperties::VISIBLE|(int)NodeFeatureProperties::SHOWING|(int)NodeFeatureProperties::ACTIVE));
            mWins.push_back(mApps[0]->addWindow("win3", "Elm_Win", {300,300,200,200}, (int)NodeFeatureProperties::VISIBLE));

            mNodes.push_back(mWins[0]->addNode("node1", "pkg1", "TeSt1234!@#$", "res.id.1", "type", "style", "", {0,0,100,100}, 0, (int)NodeFeatureProperties::SHOWING|(int)NodeFeatureProperties::ENABLED));
            mNodes.push_back(mWins[0]->addNode("node2", "pkg", "TeSt1234!@#$", "res", "type", "style", "", {100,100,200,200}, 0, (int)NodeFeatureProperties::SHOWING));
            mNodes.push_back(mWins[0]->addNode("node3", "pkg", "TeSt1234!@#$", "res", "type", "style", "", {200,200,300,300}, 0, (int)NodeFeatureProperties::SHOWING|(int)NodeFeatureProperties::ACTIVE));
            mNodes.push_back(mWins[0]->addNode("node4", "pkg", "TeSt1234!@#$", "res", "widget.type.1", "widget.style.2", "", {200,200,300,300}, 0, 0));
            mNodes.push_back(mNodes[3]->addChild("node5", "pkg", "TeSt1234!@#$", "res", "type of widget", "style of widget", "", {200,200,300,300}, 0, 0));
            mNodes.push_back(mNodes[4]->addChild("node6", "pkg", "TeSt1234!@#$", "res", "type of widget", "style of widget", "", {200,200,300,300}, 0, (int)0xFFFFFFFF));
        }

        void TearDown() override {
        }

        MockDeviceImpl *mDevice;
        MockAccessibleWatcher *mWatcher;

        std::vector<std::shared_ptr<MockAccessibleApplication>> mApps;
        std::vector<std::shared_ptr<MockAccessibleWindow>> mWins;
        std::vector<std::shared_ptr<MockAccessibleNode>> mNodes;
};


TEST_F(AurumTestUiSelector, EmptyTest)
{
    ASSERT_EQ(true, true);
}

TEST_F(AurumTestUiSelector, id_P1)
{
    auto sel = std::make_shared<UiSelector>();
    sel->id("res.id.1");
    auto objects = UiDevice::getInstance()->findObjects(sel);
    ASSERT_EQ(objects.size(), 1);
    ASSERT_EQ(objects[0]->getText(), "node1");
}

TEST_F(AurumTestUiSelector, text_P1)
{
    auto sel = std::make_shared<UiSelector>();
    sel->text("node1");
    auto objects = UiDevice::getInstance()->findObjects(sel);
    ASSERT_EQ(objects.size(), 1);
    ASSERT_EQ(objects[0]->getId(), "res.id.1");
}

TEST_F(AurumTestUiSelector, pkg_P1)
{
    auto sel = std::make_shared<UiSelector>();
    sel->pkg("pkg1");
    auto objects = UiDevice::getInstance()->findObjects(sel);
    ASSERT_EQ(objects.size(), 1);
    ASSERT_EQ(objects[0]->getText(), "node1");
}

TEST_F(AurumTestUiSelector, type_P1)
{
    auto sel = std::make_shared<UiSelector>();
    sel->type("widget.type.1");
    auto objects = UiDevice::getInstance()->findObjects(sel);
    ASSERT_EQ(objects.size(), 1);
    ASSERT_EQ(objects[0]->getText(), "node4");
}

TEST_F(AurumTestUiSelector, style_P1)
{
    auto sel = std::make_shared<UiSelector>();
    sel->style("widget.style.2");
    auto objects = UiDevice::getInstance()->findObjects(sel);
    ASSERT_EQ(objects.size(), 1);
    ASSERT_EQ(objects[0]->getText(), "node4");
}

TEST_F(AurumTestUiSelector, depth_P1)
{
    auto sel = std::make_shared<UiSelector>();
    sel->depth(2);
    auto objects = UiDevice::getInstance()->findObjects(sel);
    ASSERT_EQ(objects.size(), 1);
    ASSERT_EQ(objects[0]->getText(), "node5");
}

TEST_F(AurumTestUiSelector, minDepth_P1)
{
    auto sel = std::make_shared<UiSelector>();
    sel->minDepth(2);
    auto objects = UiDevice::getInstance()->findObjects(sel);
    ASSERT_EQ(objects.size(), 2);
    ASSERT_EQ(objects[0]->getText(), "node6");
    ASSERT_EQ(objects[1]->getText(), "node5");
}

TEST_F(AurumTestUiSelector, maxDepth_P1)
{
    auto sel = std::make_shared<UiSelector>();
    sel->maxDepth(0);
    auto objects = UiDevice::getInstance()->findObjects(sel);

    ASSERT_EQ(objects.size(), 2);
    ASSERT_EQ(objects[0]->getText(), "win1");
    ASSERT_EQ(objects[1]->getText(), "win2");
}

TEST_F(AurumTestUiSelector, isCheckable_P1)
{
    auto sel = std::make_shared<UiSelector>();
    sel->text("node6");
    auto objects = UiDevice::getInstance()->findObjects(sel);
    ASSERT_EQ(objects.size(), 1);
    ASSERT_EQ(objects[0]->isCheckable(), true);
    ASSERT_EQ(objects[0]->isChecked(), true);
    ASSERT_EQ(objects[0]->isClickable(), true);
    ASSERT_EQ(objects[0]->isEnabled(), true);
    ASSERT_EQ(objects[0]->isFocusable(), true);
    ASSERT_EQ(objects[0]->isFocused(), true);
    ASSERT_EQ(objects[0]->isLongClickable(), true);
    ASSERT_EQ(objects[0]->isScrollable(), true);
    ASSERT_EQ(objects[0]->isSelectable(), true);
    ASSERT_EQ(objects[0]->isSelected(), true);
    ASSERT_EQ(objects[0]->isShowing(), true);
    ASSERT_EQ(objects[0]->isVisible(), true);
}

TEST_F(AurumTestUiSelector, isShowing_P1)
{
    auto sel = std::make_shared<UiSelector>();
    sel->maxDepth(0);
    auto objects = UiDevice::getInstance()->findObjects(sel);

    ASSERT_EQ(objects.size(), 2);
    ASSERT_EQ(objects[0]->isShowing(), true);
    ASSERT_EQ(objects[1]->isShowing(), true);
}

TEST_F(AurumTestUiSelector, isActive_P1)
{
    auto sel = std::make_shared<UiSelector>();
    sel->maxDepth(0);
    auto objects = UiDevice::getInstance()->findObjects(sel);

    ASSERT_EQ(objects.size(), 2);
    ASSERT_EQ(objects[0]->isActive(), true);
    ASSERT_EQ(objects[1]->isActive(), true);
}

TEST_F(AurumTestUiSelector, Selector_Advanced_P1)
{
    auto sel = std::make_shared<UiSelector>();
    sel->text("win1");

    auto found = UiDevice::getInstance()->findObject(sel);
    ASSERT_EQ(found->getElementType(), "Elm_Win");
    auto children = found->getChildren();
    ASSERT_EQ(children.size(), 4);

    auto childSel = std::make_shared<UiSelector>();
    childSel->text("node5");
    childSel->depth(2);
    sel->hasChild(childSel);
    found = UiDevice::getInstance()->findObject(sel);

    ASSERT_NE(found, nullptr);
}


TEST_F(AurumTestUiSelector, Selector_Advanced_P2)
{
    auto sel = std::make_shared<UiSelector>();
    sel->text("win1");

    auto found = UiDevice::getInstance()->findObject(sel);
    ASSERT_EQ(found->getElementType(), "Elm_Win");
    auto children = found->getChildren();
    ASSERT_EQ(children.size(), 4);

    auto childSel1 = std::make_shared<UiSelector>();
    childSel1->isShowing(true);
    childSel1->isActive(true);

    auto childSel2 = std::make_shared<UiSelector>();
    childSel2->isShowing(true);
    childSel2->isEnabled(true);

    sel->hasChild(childSel1);
    sel->hasChild(childSel2);
    found = UiDevice::getInstance()->findObject(sel);

    ASSERT_NE(found, nullptr);
    ASSERT_EQ(found->getText(), "win1");
}

TEST_F(AurumTestUiSelector, Selector_Parent_P1)
{
    auto selpp = std::make_shared<UiSelector>();
    selpp->text("win1");

    auto selp = std::make_shared<UiSelector>();
    selp->text("node4")->fromParent(selpp);

    auto sel = std::make_shared<UiSelector>();
    sel->text("node5")->fromParent(selp);

    auto found = UiDevice::getInstance()->findObject(sel);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(found->getText(), "node5");
}

TEST_F(AurumTestUiSelector, Selector_Parent_N2)
{
    auto selpp = std::make_shared<UiSelector>();
    selpp->text("win1")->role("window");

    auto selp = std::make_shared<UiSelector>();
    selp->text("node4")->fromParent(selpp);

    auto sel = std::make_shared<UiSelector>();
    sel->text("node5")->fromParent(selp);

    auto found = UiDevice::getInstance()->findObject(sel);
    ASSERT_EQ(found, nullptr);

    found = UiDevice::getInstance()->findObject(selpp);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(found->getText(), "win2");

}

TEST_F(AurumTestUiSelector, Selector_Parent_N1)
{
    auto selpp = std::make_shared<UiSelector>();
    selpp->text("win2");

    auto selp = std::make_shared<UiSelector>();
    selp->text("node4")->fromParent(selpp);

    auto sel = std::make_shared<UiSelector>();
    sel->text("node5")->fromParent(selp);

    auto found = UiDevice::getInstance()->findObject(sel);
    ASSERT_EQ(found, nullptr);
}

TEST_F(AurumTestUiSelector, Selector_Advanced_N1)
{
    auto sel = std::make_shared<UiSelector>();
    sel->text("win1");

    auto found = UiDevice::getInstance()->findObject(sel);
    ASSERT_EQ(found->getElementType(), "Elm_Win");
    auto children = found->getChildren();
    ASSERT_EQ(children.size(), 4);

    auto childSel = std::make_shared<UiSelector>();
    childSel->isShowing(true);
    childSel->isActive(true);
    childSel->isCheckable(true);
    childSel->text("doesn't exist node");
    sel->hasChild(childSel);
    found = UiDevice::getInstance()->findObject(sel);

    ASSERT_EQ(found, nullptr);
}
