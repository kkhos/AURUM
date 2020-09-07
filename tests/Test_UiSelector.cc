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


class AurumTestUiSelector : public ::testing::Test {
    public:
        AurumTestUiSelector(){
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


TEST_F(AurumTestUiSelector, EmptyTest)
{
    ASSERT_EQ(true, true);
}
TEST_F(AurumTestUiSelector, id_P1)
{

}
TEST_F(AurumTestUiSelector, text_P1)
{
    
}
TEST_F(AurumTestUiSelector, pkg_P1)
{
    
}
TEST_F(AurumTestUiSelector, type_P1)
{
    
}
TEST_F(AurumTestUiSelector, style_P1)
{
    
}
TEST_F(AurumTestUiSelector, depth_P1)
{
    
}
TEST_F(AurumTestUiSelector, minDepth_P1)
{
    
}
TEST_F(AurumTestUiSelector, maxDepth_P1)
{
    
}
TEST_F(AurumTestUiSelector, isCheckable_P1)
{
    
}
TEST_F(AurumTestUiSelector, isClickable_P1)
{
    
}
TEST_F(AurumTestUiSelector, isEnabled_P1)
{
    
}
TEST_F(AurumTestUiSelector, isFocused_P1)
{
    
}
TEST_F(AurumTestUiSelector, isFocusable_P1)
{
    
}
TEST_F(AurumTestUiSelector, isScrollable_P1)
{
    
}
TEST_F(AurumTestUiSelector, isSelected_P1)
{
    
}
TEST_F(AurumTestUiSelector, isShowing_P1)
{
    
}
TEST_F(AurumTestUiSelector, isActive_P1)
{
    
}
TEST_F(AurumTestUiSelector, res_P1)
{
    
}
TEST_F(AurumTestUiSelector, desc_P1)
{
    
}
TEST_F(AurumTestUiSelector, hasChild_P1)
{
    
}
