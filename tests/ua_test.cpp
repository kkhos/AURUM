#include <gtest/gtest.h>

#include <UiDevice.h>
#include <UiObject.h>
#include <UiSelector.h>
#include <Sel.h>

class UaTest : public ::testing::Test {
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
    const UiDevice *mDevice = UiDevice::getInstance(DeviceType::DEFAULT);
    ASSERT_NE(mDevice, nullptr);
}

TEST_F(UaTest, TextSelector)
{
    std::unique_ptr<UiSelector> sel = Sel::text("test");
    ASSERT_NE(sel.get(), nullptr);
}