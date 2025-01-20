#include <gtest/gtest.h>

#include <AurumCBindings.h>

TEST(CBindingsTest, DumpScreen)
{
    aurum_init();
    const char *c_str = dump_screen();
    EXPECT_NE(c_str, nullptr);
    free_string_result(c_str);
}

TEST(CBindingsTest, FreeStringResultWorksWithNull)
{
    free_string_result(nullptr);
}