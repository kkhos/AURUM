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


class AurumTestMisc : public ::testing::Test {
    public:
        AurumTestMisc(){
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

TEST_F(AurumTestMisc, EmptyTest)
{
    ASSERT_EQ(true, true);
}
