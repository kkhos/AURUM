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


class AurumTestWaiter : public ::testing::Test {
    public:
        AurumTestWaiter(){

        }
    protected:
        void SetUp() override {

        }

        void TearDown() override {
        }
};

TEST_F(AurumTestWaiter, EmptyTest)
{
    ASSERT_EQ(true, true);
}

TEST_F(AurumTestWaiter, waitFor_P1)
{

}