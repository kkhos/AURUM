#include <gtest/gtest.h>

#include <AurumCBindings.h>

TEST(CBindingsTest, DumpScreen)
{
    aurum_init();
    const char *c_str = aurum_dump_screen();
    EXPECT_NE(c_str, nullptr);
    aurum_free_string_result(c_str);
    aurum_shutdown();
}

TEST(CBindingsTest, DumpScreenIncludeHiddenToggle)
{
    aurum_init();

    const char* withHidden = aurum_dump_screen_ex(1);
    EXPECT_NE(withHidden, nullptr);
    aurum_free_string_result(withHidden);

    const char* withoutHidden = aurum_dump_screen_ex(0);
    EXPECT_NE(withoutHidden, nullptr);
    aurum_free_string_result(withoutHidden);

    aurum_shutdown();
}

TEST(CBindingsTest, FreeStringResultWorksWithNull)
{
    aurum_free_string_result(nullptr);
}