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


class AurumTestMisc : public ::testing::Test {
    public:
        AurumTestMisc(){
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
