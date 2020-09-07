#include <gtest/gtest.h>

#include <UiDevice.h>
#include <UiObject.h>
#include <UiSelector.h>
#include <Sel.h>
#include <Until.h>

#include "MockDeviceImpl.h"

#include <iostream>
#include <algorithm>

#include <loguru.hpp>


#include "MockAccessibleWatcher.h"
#include "MockAccessibleApplication.h"
#include "MockAccessibleWindow.h"
#include "MockAccessibleNode.h"


class AurumTestUntil : public ::testing::Test {
    public:
        AurumTestUntil(){
            const char *logPath = "/tmp/ua.log";
            loguru::g_preamble = false;
            loguru::add_file(logPath, loguru::Append, loguru::Verbosity_MAX);
        }
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }
};


TEST_F(AurumTestUntil, EmptyTest)
{
    ASSERT_EQ(true, true);
}

TEST_F(AurumTestUntil, hasObject_P1)
{
    auto objectExists = Until::hasObject(Sel::text("text"));


}
TEST_F(AurumTestUntil, findObject_P1)
{
    auto objectInstance = Until::findObject(Sel::text("text"));
}
