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

TEST(CBindingsTest, FreeStringResultWorksWithNull)
{
    aurum_free_string_result(nullptr);
}