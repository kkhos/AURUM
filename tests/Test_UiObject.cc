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

using namespace Aurum;
using namespace AurumInternal::Mock;

class AurumTestUiObject : public ::testing::Test {
    public:
        AurumTestUiObject() : mDevice{nullptr},mWatcher{nullptr}, mApp{nullptr}, mWin{nullptr}, mNode{nullptr} {
        }

        void SetUp() override {
            mDevice = new MockDeviceImpl();
            UiDevice::getInstance(mDevice);

            mWatcher = new MockAccessibleWatcher();
            AccessibleWatcher::getInstance(mWatcher);

            mApp = mWatcher->addApplication("org.tizen.aurum.test.app", {0,0,1024,1024}, 0, 0);
            mWin = mApp->addWindow("title", "Elm_Win", {100,100,200,200}, (int)NodeFeatureProperties::SHOWING|(int)NodeFeatureProperties::VISIBLE|(int)NodeFeatureProperties::ACTIVE);
            mWin->addNode("test", "pkg", "TeSt1234!@#$", "res", "type", "style", "", {0,0,100,100}, 0, 0);
            mNode = mWin->addNode("test2", "pkg", "TeSt1234!@#$", "res", "type", "style", "", {100,100,200,200}, 0, 0);
            mNode->addChild("test2_1", "child", "TeSt1234!@#$", "res", "Elm_Layout", "default", "", {100,100,200,200}, 0, 0);
            mNode->addChild("test2_2", "child", "TeSt1234!@#$", "res", "Elm_Button", "default", "", {100,100,200,200}, 0, 0);
            mNode->addChild("test2_3", "child", "TeSt1234!@#$", "res", "Elm_Button", "special", "", {100,100,200,200}, 0, 0);

            mWin->addNode("ACTIVE", "pkg", "test_property", "res", "type", "style", "r.id.1", {0,0,100,100}, 0, (int)NodeFeatureProperties::ACTIVE);
            mWin->addNode("CHECKABLE", "pkg", "test_property","res", "type", "style", "r.id.2",  {0,0,100,100}, 0, (int)NodeFeatureProperties::CHECKABLE);
            mWin->addNode("CHECKED", "pkg", "test_property", "res", "type", "style", "r.id.3",  {0,0,100,100}, 0, (int)NodeFeatureProperties::CHECKED);
            mWin->addNode("FOCUSABLE", "pkg", "test_property", "res", "type", "style", "r.id.4",  {0,0,100,100}, 0, (int)NodeFeatureProperties::FOCUSABLE);
            mWin->addNode("FOCUSED", "pkg", "test_property", "res", "type", "style", "r.id.5",  {0,0,100,100}, 0, (int)NodeFeatureProperties::FOCUSED);
            mWin->addNode("INVALID", "pkg", "test_property", "res", "type", "style", "r.id.6",  {0,0,100,100}, 0, (int)NodeFeatureProperties::INVALID);
            mWin->addNode("LONGCLICKABLE", "pkg", "test_property","res", "type", "style", "r.id.7",  {0,0,100,100}, 0, (int)NodeFeatureProperties::LONGCLICKABLE);
            mWin->addNode("SCROLLABLE", "pkg", "test_property", "res", "type", "style", "r.id.8",  {0,0,100,100}, 0, (int)NodeFeatureProperties::SCROLLABLE);
            mWin->addNode("SELECTABLE", "pkg", "test_property", "res", "type", "style", "r.id.9",  {0,0,100,100}, 0, (int)NodeFeatureProperties::SELECTABLE);
            mWin->addNode("SELECTED", "pkg", "test_property", "res", "type", "style", "r.id.10",  {0,0,100,100}, 0, (int)NodeFeatureProperties::SELECTED);
            mWin->addNode("SHOWING", "pkg", "test_property", "res", "type", "style", "r.id.11",  {0,0,100,100}, 0, (int)NodeFeatureProperties::SHOWING);
            mWin->addNode("VISIBLE", "pkg", "test_property", "res", "type", "style", "r.id.12",  {0,0,100,100}, 0, (int)NodeFeatureProperties::VISIBLE);
            mWin->addNode("CLICKABLE", "pkg", "test_property", "res", "type", "style", "r.id.13",  {0,0,100,100}, 0, (int)NodeFeatureProperties::CLICKABLE);
            mWin->addNode("ENABLED", "pkg", "test_property", "res", "type", "style", "r.id.14",  {500,500,600,600}, 0, (int)NodeFeatureProperties::ENABLED);
        }

        void TearDown() override {
        }

    public:
        MockDeviceImpl *mDevice;
        MockAccessibleWatcher *mWatcher;
        std::shared_ptr<MockAccessibleApplication> mApp;
        std::shared_ptr<MockAccessibleWindow> mWin;
        std::shared_ptr<MockAccessibleNode> mNode;
};

TEST_F(AurumTestUiObject, EmptyTest)
{
    ASSERT_EQ(true, true);
}

TEST_F(AurumTestUiObject, getAccessibleNode_P1)
{
    auto obj = UiDevice::getInstance();
    auto anObj = obj->findObject(Sel::text("test"));
    ASSERT_NE(anObj, nullptr);
    auto node = anObj->getAccessibleNode();
    ASSERT_NE(node, nullptr);
    anObj = obj->findObject(Sel::text("not_exist_text"));
    ASSERT_EQ(anObj, nullptr);
}

TEST_F(AurumTestUiObject, getSelector_P1)
{
    auto obj = UiDevice::getInstance();
    auto sel = Sel::text("test");
    auto anObj = obj->findObject(sel);
    ASSERT_NE(anObj , nullptr);
    auto selOut = anObj->getSelector();
    ASSERT_EQ(sel, selOut);
}

TEST_F(AurumTestUiObject, hasObject_P1)
{
    auto obj = UiDevice::getInstance();
    auto anObj = obj->findObject(Sel::text("test2"));
    ASSERT_NE(anObj, nullptr);
    auto exists = anObj->hasObject(Sel::text("test2_1"));
    ASSERT_EQ(exists, true);
    exists = anObj->hasObject(Sel::text("test2_999"));
    ASSERT_EQ(exists, false);
}

TEST_F(AurumTestUiObject, findObject_P1)
{
    auto obj = UiDevice::getInstance();
    auto anObj = obj->findObject(Sel::text("test2"));
    ASSERT_NE(anObj, nullptr);
    auto found = anObj->findObject(Sel::type("Elm_Layout"));
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(found->getText(), "test2_1");
}

TEST_F(AurumTestUiObject, findObjects_P1)
{
    auto obj = UiDevice::getInstance();
    auto anObj = obj->findObject(Sel::text("test2"));
    ASSERT_NE(anObj, nullptr);
    auto founds = anObj->findObjects(Sel::style("default"));
    ASSERT_EQ(founds.size(), 2);
}

TEST_F(AurumTestUiObject, waitFor_P1)
{
    auto obj = UiDevice::getInstance();
    auto anObj = obj->findObject(Sel::text("test2"));
    ASSERT_NE(anObj, nullptr);
    std::thread t([&](){
        std::this_thread::sleep_for(std::chrono::milliseconds{250});
        mNode->addChild("test2_4", "pkg", "TeSt1234!@#$", "res", "Elm_Button", "waitfor", "", {100,100,200,200}, 0, 0);
    });
    auto found = anObj->waitFor(Until::hasObject(Sel::style("waitfor")));
    ASSERT_EQ(found, true);
    t.detach();
}

TEST_F(AurumTestUiObject, getParent_P1)
{
    auto obj = UiDevice::getInstance();
    auto parent = obj->findObject(Sel::text("test2"));
    ASSERT_NE(parent, nullptr);
    auto child = obj->findObject(Sel::text("test2_1"));
    ASSERT_NE(child, nullptr);
    ASSERT_EQ(parent.get()->getAccessibleNode(), child->getParent()->getAccessibleNode());
}

TEST_F(AurumTestUiObject, getChildCount_P1)
{
    auto obj = UiDevice::getInstance();
    auto parent = obj->findObject(Sel::text("test2"));
    int nChild = parent->getChildCount();
    ASSERT_EQ(nChild, 3);
}

TEST_F(AurumTestUiObject, getChildren_P1)
{
    auto obj = UiDevice::getInstance();
    auto parent = obj->findObject(Sel::text("test2"));
    auto children = parent->getChildren();
    ASSERT_EQ(children.size(), 3);
    for ( auto &&child : children) {
       ASSERT_EQ(child->getApplicationPackage(), "child");
    }
}

TEST_F(AurumTestUiObject, getApplicationPackage_P1)
{
    auto obj = UiDevice::getInstance();
    auto parent = obj->findObject(Sel::text("test2"));
    ASSERT_EQ(parent->getApplicationPackage(), "pkg");
}

TEST_F(AurumTestUiObject, getId)
{
    auto obj = UiDevice::getInstance();
    auto parent = obj->findObject(Sel::text("test2"));
    ASSERT_EQ(parent->getId(), "res");
}

TEST_F(AurumTestUiObject, getAutomationId)
{
    auto obj = UiDevice::getInstance();
    auto parent = obj->findObject(Sel::text("test2"));
    ASSERT_EQ(parent->getAutomationId(), "");
}

TEST_F(AurumTestUiObject, getType_P1)
{
    auto obj = UiDevice::getInstance();
    auto parent = obj->findObject(Sel::text("test2"));
    ASSERT_EQ(parent->getType(), "type");
}

TEST_F(AurumTestUiObject, getElementStyle_P1)
{
    auto obj = UiDevice::getInstance();
    auto parent = obj->findObject(Sel::text("test2"));
    ASSERT_EQ(parent->getElementStyle(), "style");
}

TEST_F(AurumTestUiObject, getText_P1)
{
    auto obj = UiDevice::getInstance();
    auto parent = obj->findObject(Sel::text("test2"));
    ASSERT_EQ(parent->getText(), "test2");
}

TEST_F(AurumTestUiObject, getRole_P1)
{
    auto obj = UiDevice::getInstance();
    auto parent = obj->findObject(Sel::text("test2"));
    ASSERT_EQ(parent->getRole(), "TeSt1234!@#$");
}

TEST_F(AurumTestUiObject, setText_P1)
{
    auto obj = UiDevice::getInstance();
    auto parent = obj->findObject(Sel::text("test2"));
    parent->setText("new_test2");
    ASSERT_EQ(parent->getText(), "new_test2");
}

TEST_F(AurumTestUiObject, getScreenBoundingBox_P1)
{
    auto obj = UiDevice::getInstance();
    auto parent = obj->findObject(Sel::text("test2"));
    auto box = parent->getScreenBoundingBox();

    ASSERT_EQ(box.mBottomRight.x, 200 );
    ASSERT_EQ(box.mBottomRight.y, 200 );
    ASSERT_EQ(box.mTopLeft.x, 100 );
    ASSERT_EQ(box.mTopLeft.y, 100 );
}

TEST_F(AurumTestUiObject, getDescendant_P1)
{
    auto obj = UiDevice::getInstance();
    auto parent = obj->findObject(Sel::text("test2"));
    auto descendant = parent->getDescendant();

    ASSERT_EQ(descendant->mChildren.size(), 3);
    ASSERT_EQ(descendant->mNode->getText(), "test2");

    for ( auto &&node : descendant->mChildren){
        ASSERT_EQ(node->mNode->getApplicationPackage(), "child");
        ASSERT_EQ(node->mNode->getRole(), "TeSt1234!@#$");
    }
}

TEST_F(AurumTestUiObject, isChecked_P1)
{
    auto obj = UiDevice::getInstance();
    auto win = obj->findObject(Sel::text("title"));
    auto sel = std::make_shared<UiSelector>();
    sel->isChecked(true);
    auto found = win->findObject(sel);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(found->getText(), "CHECKED");
    ASSERT_EQ(found->isChecked(), true);
}

TEST_F(AurumTestUiObject, isCheckable_P1)
{
    auto obj = UiDevice::getInstance();
    auto win = obj->findObject(Sel::text("title"));
    auto sel = std::make_shared<UiSelector>();
    sel->isCheckable(true);
    auto found = win->findObject(sel);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(found->getText(), "CHECKABLE");
    ASSERT_EQ(found->isCheckable(), true);
}

TEST_F(AurumTestUiObject, isEnalbed_P1)
{
        auto obj = UiDevice::getInstance();
    auto win = obj->findObject(Sel::text("title"));
    auto sel = std::make_shared<UiSelector>();
    sel->isEnabled(true);
    auto found = win->findObject(sel);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(found->getText(), "ENABLED");
    ASSERT_EQ(found->isEnabled(), true);
}

TEST_F(AurumTestUiObject, isFocusable_P1)
{
        auto obj = UiDevice::getInstance();
    auto win = obj->findObject(Sel::text("title"));
    auto sel = std::make_shared<UiSelector>();
    sel->isFocusable(true);
    auto found = win->findObject(sel);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(found->getText(), "FOCUSABLE");
    ASSERT_EQ(found->isFocusable(), true);
}

TEST_F(AurumTestUiObject, isFocused_P1)
{    auto obj = UiDevice::getInstance();
    auto win = obj->findObject(Sel::text("title"));
    auto sel = std::make_shared<UiSelector>();
    sel->isFocused(true);
    auto found = win->findObject(sel);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(found->getText(), "FOCUSED");
    ASSERT_EQ(found->isFocused(), true);
}

TEST_F(AurumTestUiObject, isScrollable_P1)
{
    auto obj = UiDevice::getInstance();
    auto win = obj->findObject(Sel::text("title"));
    auto sel = std::make_shared<UiSelector>();
    sel->isScrollable(true);
    auto found = win->findObject(sel);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(found->getText(), "SCROLLABLE");
    ASSERT_EQ(found->isScrollable(), true);
}

TEST_F(AurumTestUiObject, isSelectable_P1)
{
    auto obj = UiDevice::getInstance();
    auto win = obj->findObject(Sel::text("title"));
    auto sel = std::make_shared<UiSelector>();
    sel->isSelectable(true);
    auto found = win->findObject(sel);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(found->getText(), "SELECTABLE");
    ASSERT_EQ(found->isSelectable(), true);
}

TEST_F(AurumTestUiObject, isSelected_P1)
{
    auto obj = UiDevice::getInstance();
    auto win = obj->findObject(Sel::text("title"));
    auto sel = std::make_shared<UiSelector>();
    sel->isSelected(true);
    auto found = win->findObject(sel);
    ASSERT_NE(found, nullptr);
    ASSERT_EQ(found->getText(), "SELECTED");
    ASSERT_EQ(found->isSelected(), true);
}

TEST_F(AurumTestUiObject, isVisible_P1)
{
    auto obj = UiDevice::getInstance();
    auto win = obj->findObject(Sel::text("title"));
    auto sel = std::make_shared<UiSelector>();
    sel->isVisible(true);
    auto founds = win->findObjects(sel);
    ASSERT_NE(founds[1], nullptr);
    ASSERT_EQ(founds[1]->getText(), "VISIBLE");
    ASSERT_EQ(founds[1]->isVisible(), true);
}

TEST_F(AurumTestUiObject, isShowing_P1)
{
    auto obj = UiDevice::getInstance();
    auto win = obj->findObject(Sel::text("title"));
    auto sel = std::make_shared<UiSelector>();
    sel->isShowing(true);
    auto founds = win->findObjects(sel);
    ASSERT_NE(founds[1], nullptr);
    ASSERT_EQ(founds[1]->getText(), "SHOWING");
    ASSERT_EQ(founds[1]->isShowing(), true);
}

TEST_F(AurumTestUiObject, isActive_P1)
{
    auto obj = UiDevice::getInstance();
    auto win = obj->findObject(Sel::text("title"));
    auto sel = std::make_shared<UiSelector>();
    sel->isActive(true);
    auto founds = win->findObjects(sel);
    ASSERT_NE(founds[1], nullptr);
    ASSERT_EQ(founds[1]->getText(), "ACTIVE");
    ASSERT_EQ(founds[1]->isActive(), true);
}

TEST_F(AurumTestUiObject, click_P1)
{
    auto obj = UiDevice::getInstance()->findObject(Sel::text("ENABLED"));
    ASSERT_NE(obj, nullptr);
    obj->click();

    auto rect = obj->getScreenBoundingBox();
    const Point2D<int> midPoint = rect.midPoint();

    ASSERT_EQ(mDevice->mTouchRelease[1].x, 0);
    ASSERT_EQ(mDevice->mTouchRelease[1].y, 0);
}

TEST_F(AurumTestUiObject, longClick_P1)
{
    int interval = 1500;
    auto obj = UiDevice::getInstance()->findObject(Sel::text("ENABLED"));
    ASSERT_NE(obj, nullptr);
    obj->longClick(interval);

    auto rect = obj->getScreenBoundingBox();
    const Point2D<int> midPoint = rect.midPoint();

    ASSERT_NEAR(mDevice->mTouchRelease[1].stamp1, mDevice->mTouchRelease[1].stamp2, interval*1000*1.1);
    ASSERT_EQ(mDevice->mTouchRelease[1].x, 0);
    ASSERT_EQ(mDevice->mTouchRelease[1].y, 0);

}

TEST_F(AurumTestUiObject, DoAtspiActivate_P1)
{
    auto obj = UiDevice::getInstance()->findObject(Sel::text("ENABLED"));
    ASSERT_NE(obj, nullptr);
    obj->DoAtspiActivate();
}

TEST_F(AurumTestUiObject, getParent_N1)
{
    auto obj = UiDevice::getInstance();
    auto parent = obj->findObject(Sel::text("test2"));
    ASSERT_NE(parent, nullptr);
    auto child = obj->findObject(Sel::text("test2_2"));
    ASSERT_NE(child, nullptr);
    ASSERT_NE(child->getParent()->getAccessibleNode(), nullptr);
}

TEST_F(AurumTestUiObject, getChildCount_N1)
{
    auto obj = UiDevice::getInstance();
    auto parent = obj->findObject(Sel::text("test2"));
    int nChild = parent->getChildCount();
    ASSERT_NE(nChild, 0);
}

TEST_F(AurumTestUiObject, getChildren_N1)
{
    auto obj = UiDevice::getInstance();
    auto parent = obj->findObject(Sel::text("test2"));
    auto children = parent->getChildren();
    ASSERT_EQ(children.size(), 3);
    for ( auto &&child : children) {
       ASSERT_NE(child->getApplicationPackage(), "test");
    }
}

TEST_F(AurumTestUiObject, getApplicationPackage_N1)
{
    auto obj = UiDevice::getInstance();
    auto parent = obj->findObject(Sel::text("test2"));
    ASSERT_NE(parent->getApplicationPackage(), "win");
}

TEST_F(AurumTestUiObject, getId_N1)
{
    auto obj = UiDevice::getInstance();
    auto parent = obj->findObject(Sel::text("test2"));
    ASSERT_NE(parent->getId(), "test");
}

TEST_F(AurumTestUiObject, getAutomationId_N1)
{
    auto obj = UiDevice::getInstance();
    auto parent = obj->findObject(Sel::text("test2"));
    ASSERT_NE(parent->getAutomationId(), "test");
}

TEST_F(AurumTestUiObject, getType_N1)
{
    auto obj = UiDevice::getInstance();
    auto parent = obj->findObject(Sel::text("test2"));
    ASSERT_NE(parent->getType(), "test");
}

TEST_F(AurumTestUiObject, getElementStyle_N1)
{
    auto obj = UiDevice::getInstance();
    auto parent = obj->findObject(Sel::text("test2"));
    ASSERT_NE(parent->getElementStyle(), "test");
}

TEST_F(AurumTestUiObject, getText_N1)
{
    auto obj = UiDevice::getInstance();
    auto parent = obj->findObject(Sel::text("test2"));
    ASSERT_NE(parent->getText(), "test");
}

TEST_F(AurumTestUiObject, getRole_N1)
{
    auto obj = UiDevice::getInstance();
    auto parent = obj->findObject(Sel::text("test2"));
    ASSERT_NE(parent->getRole(), "test");
}

TEST_F(AurumTestUiObject, setText_PN)
{
    auto obj = UiDevice::getInstance();
    auto parent = obj->findObject(Sel::text("test2"));
    parent->setText("new_test2");
    ASSERT_NE(parent->getText(), "new_test");
}

TEST_F(AurumTestUiObject, getScreenBoundingBox_N1)
{
    auto obj = UiDevice::getInstance();
    auto parent = obj->findObject(Sel::text("test2"));
    auto box = parent->getScreenBoundingBox();

    ASSERT_NE(box.mBottomRight.x, 0 );
    ASSERT_NE(box.mBottomRight.y, 0 );
    ASSERT_NE(box.mTopLeft.x, 0 );
    ASSERT_NE(box.mTopLeft.y, 0 );
}
